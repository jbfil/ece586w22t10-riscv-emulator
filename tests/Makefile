# ECE586 - Julia Filipchuk
#

rvdir     = /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain
rvgcc     = $(rvdir) /bin/riscv64-unknown-linux-gnu-gcc
rvobjdump = $(rvdir)/bin/riscv64-unknown-linux-gnu-objdump
rvas      = $(rvdir)/bin/riscv64-unknown-linux-gnu-as

asm_files = $(wildcard *.s)
obj_files = $(patsubst %.s,%.o,  $(asm_files))
mem_files = $(patsubst %.s,%.mem,$(asm_files))
res_files = $(patsubst %.mem,%.res,$(wildcard *.mem))




all: mem



obj: $(obj_files)
	

mem: $(mem_files)


clean:
	rm -rf $(mem_files)
	rm -rf $(obj_files)
	rm -rf $(res_files)

check: $(res_files)
	


.PHONY: obj mem clean all check


%.o: %.s
	$(rvas) $< -o $@

%.mem: %.o
	$(rvobjdump) -d $< | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $@

%.res: %.mem
	../rvemu $(RFLAGS) -i $< > $@

