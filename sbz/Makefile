CFLAGS = -pedantic -std=c11 -D_XOPEN_SOURCE=700 -Wall
CC = gcc

sources = $(wildcard */*/*.c)
outs = $(sources:%.c=%.out)
debugs = $(sources:%.c=%.debug)

.PHONY: outs debugs clean

outs: $(outs)

debugs: $(debugs)

clean: 
	rm -f $(outs) $(debugs)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.debug: %.c
	$(CC) $(CFLAGS) -g -o $@ $<
