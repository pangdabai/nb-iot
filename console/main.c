#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

static int handle = -1;
static void *readerTask(void *arg) {
	uint8_t buf[2048];
	int len = 0;
	while (true) {
		len = read(handle, buf, sizeof(buf));
		if (len <= 0) continue;
		if (len == 1 && buf[0] == '\n') continue;
		if (buf[len-1] == '\n') buf[len-1] = 0;
		else buf[len] = 0;
		puts((char *)buf);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: console PORT_PATH\n");
		exit(-1);
	} else {
		handle = open(argv[1], O_RDWR | O_NOCTTY);
		if (handle < 0) {
			fprintf(stderr, "failed to open \"%s\"\n", argv[1]);
		}
	}

	unsigned long h;
	pthread_create(&h, NULL, readerTask, NULL);

	char cmd[2048];
	while (true) {
		scanf("%s", cmd);
		write(handle, cmd, strlen(cmd));
		write(handle, "\n", 1);
	}
	return 0;
}
