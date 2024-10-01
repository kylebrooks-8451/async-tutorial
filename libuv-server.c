#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <uv.h>

void signal_handler(int signal) {
    _exit(0);
}

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
    counter++;

    if (counter >= 10e6)
        uv_idle_stop(handle);
}

int main() {
    uv_loop_t *loop;
    uv_tcp_t server;
    uv_idle_t idler;

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        return EXIT_FAILURE;
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("signal");
        return EXIT_FAILURE;
    }

    loop = uv_default_loop();
    uv_tcp_init(loop, &server);

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling...\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}
