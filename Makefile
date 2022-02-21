
CC = gcc
CFLAGS += -Wall

run: rvemu
	./rvemu -v -r -i tests/test.txt

rvemu: rvemu.o common.o


common.o: common.c common.h

