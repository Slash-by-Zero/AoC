CFLAGS = -g -D_XOPEN_SOURCE=700 -std=c11 -pedantic -Wall
CC = gcc

.PHONY: all clean

all: testbench

clean:
	rm -f *.o
	rm -f *.gch

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%: %.o
	$(CC) $(CFLAGS) -o ../$@ $^

testbench: testbench.o cmdline.o -lm

testbench.o: testbench.c cmdline.h

cmdline.o: cmdline.c cmdline.h
