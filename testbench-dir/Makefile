CFLAGS = -D_XOPEN_SOURCE=700 -std=c11 -pedantic -Wall
CC = gcc

.PHONY: all debug clean

debug: CFLAGS += -g
debug: all
all: testbench.out

clean:
	rm -f *.o
	rm -f *.gch

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.out: %.o
	$(CC) $(CFLAGS) -o ../$@ $^

testbench.out: testbench.o cmdline.o -lm

testbench.o: testbench.c cmdline.h

cmdline.o: cmdline.c cmdline.h
