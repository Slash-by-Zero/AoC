CFLAGS = -pedantic -std=c11 -D_XOPEN_SOURCE=700 -Wall -g
SFLAGS = -nolibc -nostdlib 
CC = gcc

csources = $(wildcard */*/*/*.c)
asmsources = $(wildcard */*/*/*.S)
jssources = $(wildcard */*/*/*.js)
outs = $(csources:%.c=%.out) $(asmsources:%.S=%.out) $(jssources:%.js=%.out)

.PHONY: o clean

o: $(outs)

clean: 
	rm -f $(outs)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $<
	
%.out: %.S
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $<

define bash
#!/bin/bash
node 
endef

%.out:%.js
	echo "#!/bin/bash\nnode $(notdir $<)" > $@
	chmod u+x $@
