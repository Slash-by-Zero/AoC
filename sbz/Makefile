CFLAGS = -pedantic -std=c11 -D_XOPEN_SOURCE=700 -Wall
SFLAGS = -nostartfiles -nostdlib -nodefaultlibs -nolibc
CC = gcc

sources = $(wildcard */*/*.c) \
	$(wildcard */*/*.S)
outs = $(filter %.out, $(sources:%.c=%.out) \ $(sources:%.S=%.out))
debugs = $(outs:%.out=%.gout)

.PHONY: o g clean

o: $(outs)

g: CFLAGS += -g
g: $(debugs)

clean: 
	rm -f $(outs)
	rm -f $(debugs)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $<
	
%.out: %.S
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $<

%.gout: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.gout: %.S
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $<
