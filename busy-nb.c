#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>

int main() {
    int f;
    ssize_t bytesRead;
    char buffer[1024];

    // Open the current terminal in non-blocking mode
    if ((f = open("/dev/tty", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("open");
        return 1;
    }

    // Read returns 0 for EOF, -1 for error
    // Note that -1 and errno == EAGAIN is normal in non-blocking mode
    // Loop until EOF
    while ((bytesRead = read(f, buffer, 1024))) {
        if (bytesRead == -1) {
            // Note on Linux EAGAIN == EWOULDBLOCK
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            } else {
                perror("read");
                return 1;
            }
        }
        printf("%.1024s", buffer);
    }
    return 0;
}
