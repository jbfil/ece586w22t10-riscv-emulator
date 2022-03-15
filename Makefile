
CC = gcc
#CFLAGS += -Wall

HEADERS = $(wildcard *.h)


all: rvemu
	

clean:
	rm -rf rvemu
	rm -rf *.o

run: rvemu
	./rvemu -d -r -i jtests/1_nop.mem

step: rvemu
	./rvemu -d -r --step -i jtests/1_nop.mem

tests:
	$(MAKE) -C jtests

.PHONY: all run tests clean step
	

rvemu: rvemu.o common.o inst.o inst_jb.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
