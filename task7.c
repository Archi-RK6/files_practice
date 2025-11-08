#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main(){
	int fd1, fd2;
    	char path1[256], path2[256];

    	printf("Enter first file path: ");
    	scanf("%255s", path1);

    	printf("Enter second file path: ");
    	scanf("%255s", path2);

    	fd1 = open(path1, O_RDONLY);
    	if (fd1 == -1) {
        	perror("open first");
        	return -1;
    	}

    	fd2 = open(path2, O_RDONLY);
    	if (fd2 == -1) {
        	perror("open second");
        	close(fd1);
        	return -1;
    	}

	char buf1[256];
    	char buf2[256];
    	ssize_t r1, r2;
    	long long index = 0;

    	while (1) {
        	r1 = read(fd1, buf1, sizeof(buf1));
        	if (r1 == -1) {
            		perror("read first");
            		close(fd1);
            		close(fd2);
            		return -1;
        	}

        	r2 = read(fd2, buf2, sizeof(buf2));
        	if (r2 == -1) {
            		perror("read second");
        		close(fd1);
            		close(fd2);
            		return -1;
        	}
		
		if (r1 == 0 && r2 == 0) {
            		printf("Files are identical\n");
            		close(fd1);
            		close(fd2);
            		return 0;
        	}

		size_t m;
		if(r1 < r2){
			m = r1;
		}else{
			m = r2;
		}

		for (ssize_t i = 0; i < m; i++) {
            		if (buf1[i] != buf2[i]) {
                	printf("Different byte %lld\n", index + i);
                	close(fd1);
                	close(fd2);
                	return 1;
            		}
        	}

		if (r1 != r2) {
            		printf("Files differ at byte %lld\n", index + m);
            		close(fd1);
            		close(fd2);
            		return 1;
        	}

		index += m;
		
	}
	close(fd1);
    	close(fd2);
    	return 0;
}
