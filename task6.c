#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

int main(){
	int fd;
	const char *filename = "numbers.txt";

	fd  = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
        	perror("open");
        	return 1;
    	}

	char buf[64];
	ssize_t wr;
    	for (int i = 1; i <= 10; i++) {
        	int n = snprintf(buf, sizeof(buf), "%d\n", i);
		wr = write(fd, buf, n);
        	if (wr == -1) {
        	   	perror("write");
        		close(fd);
        		return -1;
        	}
    	}
	close(fd);

	fd = open(filename, O_RDWR);
    	if (fd == -1) {
        	perror("open");
        	return -1;
	}

    	int line = 1;
    	off_t line_start = -1;
    	off_t line_ending = -1;
    	char ch;
    	ssize_t r;
	while (true){
		r = read(fd, &ch, 1);
		if(r == -1) {
			perror("read");
			close(fd);
			return -1;
		}
		if(r==0) {
			break;
		}

		if(ch == '\n'){
			line ++;
		}
	}

	if (lseek(fd, line_start, SEEK_SET) == (off_t)-1) {
        	perror("lseek");
        	close(fd);
        	return -1;
    	}

    	wr = write(fd, "100\n", 4);
    	if (wr == -1) {
        	perror("write");
        	close(fd);
        	return -1;
    	}

	if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        	perror("lseek");
        	close(fd);
        	return -1;
    	}

    	char readbuf[128];
    	while ((r = read(fd, readbuf, sizeof(readbuf))) != 0) {
    	    	if (r == -1) {
        	perror("read");
        	close(fd);
        	return -1;
        	}
        	ssize_t out = write(STDOUT_FILENO, readbuf, r);
        	if (out == -1) {
            		perror("write");
            		close(fd);
            		return -1;
        	}
    	}

    	close(fd);
    	return 0;
	
}
