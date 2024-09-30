#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

int main() {
    int sfd, cfd;
    struct sockaddr_in server, client;
    socklen_t clientLen;
    ssize_t bytesRead;
    char buffer[1024];
    struct pollfd fds[1];

    // Create TCP socket
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8451);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    // Only allow 1 pending connection
    if (listen(sfd, 1) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    // Accept the connection
    clientLen = sizeof(client);
    if ((cfd = accept(sfd, (struct sockaddr *) &client, &clientLen)) == -1) {
        perror("accept");
        return EXIT_FAILURE;
    }

    // Set the client socket to non-blocking
    if (fcntl(cfd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        return EXIT_FAILURE;
    }

    // Poll for input or data available to read
    fds[0].fd = cfd;
    fds[0].events = POLLIN;

    // Poll returns -1 for error, 0 for timeout, or the number of fds with events
    while (poll(fds, 1, -1) != -1) {
        // Check for POLLIN event, should always be true
        if ((fds[0].revents & POLLIN) || (fds[0].revents & POLLHUP)) {
            bytesRead = read(cfd, buffer, 1024);
            if (bytesRead == -1) {
                // Note on Linux EAGAIN == EWOULDBLOCK
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    continue;
                } else {
                    perror("read");
                    return EXIT_FAILURE;
                }
            }
            // EOF
            if (bytesRead == 0) {
                return EXIT_SUCCESS;
            }
            printf("%.*s", (int) bytesRead, buffer);
        }
    }

    perror("poll");
    return EXIT_FAILURE;
}
