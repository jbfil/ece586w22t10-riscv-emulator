
CC = gcc
CFLAGS += -Wall

all: rvemu
	

clean:
	rm -rf rvemu

run: rvemu
	./rvemu -v -r -i jtests/1_nop.mem

tests:
	$(MAKE) -C jtests

.PHONY: all run tests clean
	

rvemu: rvemu.o common.o


common.o: common.c common.h

