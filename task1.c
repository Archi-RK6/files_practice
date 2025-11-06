#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    int fd1, fd2;
    char src[256];
    char dst[256];

    printf("Enter source path: ");
    if (scanf("%255s", src) != 1) {
        fprintf(stderr, "input error\n");
        return 1;
    }

    printf("Enter destination path: ");
    if (scanf("%255s", dst) != 1) {
        fprintf(stderr, "input error\n");
        return 1;
    }

    fd1 = open(src, O_RDONLY);
    if (fd1 == -1) {
        perror("open source");
        return 1;
    }

    fd2 = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("open destination");
        close(fd1);
        return 1;
    }

    char buf[1024];
    ssize_t ret;
    size_t total_bytes = 0;

    while ((ret = read(fd1, buf, sizeof(buf))) != 0) {
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            close(fd1);
            close(fd2);
            return 1;
        }

        ssize_t written = 0;
        while (written < ret) {
            ssize_t nr = write(fd2, buf + written, ret - written);
            if (nr == -1) {
                if (errno == EINTR)
                    continue;
                perror("write");
                close(fd1);
                close(fd2);
                return 1;
            }
            written += nr;
        }

        total_bytes += (size_t)ret;
    }

    printf("Copied %zu bytes\n", total_bytes);

    close(fd1);
    close(fd2);
    return 0;
}
