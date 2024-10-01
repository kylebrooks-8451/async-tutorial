TARGETS = blocking busy-nb poll-nb
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGETS)
all: default

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<

libuv-server:
	$(CC) $(CFLAGS) $(LIBS) -o libuv-server libuv-server.c

.PRECIOUS: $(TARGETS) libuv-server

clean:
	-rm -f $(TARGETS)
	-rm -f libuv-server
