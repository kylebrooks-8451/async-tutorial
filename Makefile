TARGETS = blocking busy-nb poll-nb
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGETS)
all: default

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PRECIOUS: $(TARGETS)

clean:
	-rm -f $(TARGETS)
