#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>

int main() {
    int f;
    ssize_t bytesRead;
    char buffer[1024];
    struct pollfd fds[1];
    int ready;

    // Open the current terminal in non-blocking mode
    if ((f = open("/dev/tty", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("open");
        return 1;
    }

    // Poll for input or data available to read
    fds[0].fd = f;
    fds[0].events = POLLIN;

    // Poll returns -1 for error, 0 for timeout, or the number of fds with events
    while ((ready = poll(fds, 1, -1)) != -1) {
        printf("Ready: %d\n", ready);
        // Check for POLLIN event, should always be true
        if (fds[0].revents & POLLIN) {
            printf("reading bytes\n");
            bytesRead = read(f, buffer, 1024);
            if (bytesRead == -1) {
                // Note on Linux EAGAIN == EWOULDBLOCK
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    continue;
                } else {
                    perror("read");
                    return 1;
                }
            }
            // EOF
            if (bytesRead == 0) {
                return 0;
            }
            printf("%.1024s", buffer);
        }
    }

    perror("poll");
    return 1;
}
