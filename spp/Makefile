CC ?= gcc
CFLAGS ?= -std=gnu99 -Wall -pedantic -g
LIBS = -lresolv

RESOLV_OBJS = mfdnscheck
SIMPLE_OBJS = authlog ifauthnext tarpit
TARGETS = $(RESOLV_OBJS) $(SIMPLE_OBJS)

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

$(RESOLV_OBJS):
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c -lresolv

$(SIMPLE_OBJS):
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c
