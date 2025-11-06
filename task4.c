#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(void) {
 	int fd;
    	const char *filename = "log.txt";

    	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    	if (fd == -1) {
        	perror("open");
        	return 1;
    	}

    	char buf[256];
    	ssize_t r;

    
    	while ((r = read(STDIN_FILENO, buf, sizeof(buf) - 1)) == -1 && errno == EINTR) {}

    	if (r == -1) {
        	perror("read stdin");
        	close(fd);
        	return 1;
    	}

    	buf[r] = '\0';

    	size_t len = 0;
    	while (len < (size_t)r && buf[len] != '\n' && buf[len] != '\r') {
        	len++;
    	}
    	buf[len] = '\0';

    	char output[512];
    	int n = snprintf(output, sizeof(output), "PID=%ld: %s\n", (long)getpid(), buf);
    	if (n < 0 || (size_t)n >= sizeof(output)) {
        	fprintf(stderr, "line too long\n");
        	close(fd);
        	return 1;
    	}

    	ssize_t nr = write(fd, output, n);
    	if (nr == -1) {
        	perror("write");
        	close(fd);
         	return 1;
    	}

    	off_t pos = lseek(fd, 0, SEEK_CUR);
    	if (pos == (off_t)-1) {
        	perror("lseek");
        	close(fd);
        	return 1;
	}
	
    	printf("New file position: %lld\n", (long long)pos);

    	/* */

    	close(fd);
    	return 0;
}

