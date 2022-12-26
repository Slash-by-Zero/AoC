CFLAGS = -pedantic -std=c11 -D_XOPEN_SOURCE=700 -Wall
CC = gcc

sources = $(wildcard */*/*.c) \
	$(wildcard */*/*.S
outs = $(sources:%.c=%.out) \
	$(sources:%.S=%.out
debugs = $(outs:%.out=%.gout)

.PHONY: o g clean

o: $(outs)

g: CFLAGS += -g
g: $(debugs)

clean: 
	rm -f $(outs)
	rm -f $(debugs)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^
	
%.out: %.S
	$(CC) $(CFLAGS) -o $@ $^

%.gout: %.S
	$(CC) $(CFLAGS) -o $@ $<

%.gout: %.S
	$(CC) $(CFLAGS) -o $@ $<
