
CC = gcc
#CFLAGS += -Wall
HEADERS = $(wildcard *.h)

TESTS = $(wildcard *.mem)
mem_files = $(patsubst %.s,%.mem,$(asm_files))

all: rvemu
	

clean:
	rm -rf rvemu
	rm -rf *.o

run: rvemu
	./rvemu $(RFLAGS)

step: rvemu
	./rvemu $(RFLAGS) --step

tests:
	$(MAKE) -C tests
	$(MAKE) -C jtests

check: rvemu tests
	$(MAKE) -C tests check
	$(MAKE) -C jtests check

.PHONY: all run tests clean step check
	

rvemu: rvemu.o common.o inst.o inst_jb.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.res: %.mem
	./rvemu $(RFLAGS) -i $< > $@
