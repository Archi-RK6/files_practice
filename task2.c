#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
	int fd1;
    	const char *src = "data.txt";

    	fd1 = open(src, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    	if (fd1 == -1) {
        	perror("open");
        	return 1;
    	}

    	ssize_t nr = write(fd1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
    	if (nr != 26) {
        	if (nr == -1)
            		perror("write");
        	else
            		fprintf(stderr, "short write\n");
        	close(fd1);
        	return 1;
    	}
    	if (close(fd1) == -1) {
        	perror("close");
        	return 1;
    	}

    	fd1 = open(src, O_RDWR);
    	if (fd1 == -1) {
        	perror("open");
        	return 1;
    	}

    	off_t size = lseek(fd1, 0, SEEK_END);
    	if (size == (off_t)-1) {
        	perror("lseek");
        	close(fd1);
        	return 1;
    	}
    	printf("Size before truncate: %lld\n", (long long)size);

    	if (ftruncate(fd1, 10) == -1) {
        	perror("ftruncate");
        	close(fd1);
        	return 1;
    	}

    	off_t size1 = lseek(fd1, 0, SEEK_END);
    	if (size1 == (off_t)-1) {
        	perror("lseek");
        	close(fd1);
        	return 1;
    	}
    	printf("Size after truncate:  %lld\n", (long long)size1);

    	if (lseek(fd1, 0, SEEK_SET) == (off_t)-1) {
        	perror("lseek");
        	close(fd1);
        	return 1;
    	}

    	char buf[1025];
	ssize_t r = read(fd1, buf, 1024);
	if (r == -1) {
		perror("read");
    		close(fd1);
    		return 1;
	}

	if (r > 0) {
    		buf[r] = '\0';
    		printf("%s", buf);
	}
	printf("\n");


    	if (close(fd1) == -1) {
        	perror("close");
        	return 1;
    	}

    	return 0;
}
