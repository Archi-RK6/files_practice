#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(){
	int fd; 
	const char * filename = "sparse.bin";

	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1) {
        	perror("open");
        	return -1;
    	}

	ssize_t wr = write(fd, "START", 5);
	if (wr == -1)
    	{
        	perror("write");
    		close(fd);
		return -1;
	}

	off_t l_ret = lseek(fd, 1024*1024, SEEK_CUR);
	if (l_ret == (off_t) - 1){
		perror("lseek");
		close(fd);
		return -1;
	}

	wr = write(fd, "END", 3);
        if (wr == -1)
        {
                perror("write");
                close(fd);
                return -1;
        }

	close(fd);

	fd = open(filename, O_RDONLY);
	if(fd == -1) {
		perror("open");
		return -1;
	}

	l_ret = lseek(fd, 0, SEEK_END);
	if (l_ret == (off_t) - 1){
                perror("lseek");
                close(fd);
                return -1;
        }

	printf("File size = %lld\n", (long long)l_ret);
	
	/* */
	
	close(fd);
	
	return 0;
}
