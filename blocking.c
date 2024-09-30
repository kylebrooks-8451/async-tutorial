#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int f;
    ssize_t bytesRead;
    char buffer[1024];

    // Open the current terminal
    if ((f = open("/dev/tty", O_RDONLY)) == -1) {
        perror("open");
        return 1;
    }

    // Read returns 0 for EOF, -1 for error
    while ((bytesRead = read(f, buffer, 1024)) > 0) {
        printf("%.1024s", buffer);
    }
    if (bytesRead == -1) {
        perror("read");
        return 1;
    }
    return 0;
}
