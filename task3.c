#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    	int fd;
	char src[256];
    	
	printf("Enter source path: ");
    	if (scanf("%255s", src) != 1) {
        	fprintf(stderr, "input error\n");
        	return 1;
    	}
	
	fd = open(src, O_RDONLY);
    	if (fd == -1) {
        	perror("open source");
        	return 1;
   	}

	off_t size = lseek(fd, 0, SEEK_END);
    	if (size == (off_t)-1) {
        	perror("lseek end");
        	close(fd);
        	return 1;
    	}

	if (size == 0) {
        	printf("\n");
        	close(fd);
        	return 1;
    	}

	char ch;
	for(off_t pos = size; pos > 0; pos--){
		if(lseek(fd, pos - 1, SEEK_SET) == (off_t)-1) {
			perror("lseek");
			close(fd);
			return 1;
		}
		
		ssize_t r;
        	while ((r = read(fd, &ch, 1)) == -1 && errno == EINTR) {}
		if (r == -1) {
			perror("read");
			close (fd);
			return 1;
		}

		if (printf("%c", ch) < 0) {
    			perror("printf");
    			close(fd);
    			return 1;
		}
	}

	
	close(fd);
	return 0;
}

