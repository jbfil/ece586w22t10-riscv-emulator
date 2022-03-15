# ece586w22t10-riscv-emulator
Project repository.

```
Usage: rvemu [options]
   or: rvemu            # Load program.mem and run in silent mode.
   or: rvemu -v         # Load program.mem and run in verbose mode.
   or: rvemu -d         # Load program.mem and run in debug mode.
   or: rvemu -i <file>  # Load <file> and run in silent mode.

Options:
  -v --vervose       Enable verbose mode. Display hex of each instruction, final PC.
  -d --debug         Enable additional debug information.
  -r --echo-mem      Display all lines read from memory file.
  -i <file>, --input <file> 
                     Read mem file instead of program.mem.
  -s <addr>, --start_addr <addr>
                     Set starting PC address for program counter.
  -S <addr>, --stack_addr <addr>
                     Set starting SP address stack pointer.
```

## Compiling

Present makefile can be run to build project and mem files for tests.

```bash
make       # builds rvemu
```

```bash
make run   # builds and runs
```

```bash
make tests # builds tests
```

