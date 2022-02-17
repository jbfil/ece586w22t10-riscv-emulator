
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t	u08;
typedef uint32_t u32;
typedef int32_t	s32;

// Memory Model
union {
	u08	bytes[1 << 16]; // View as bytes.
	u32	words[1 << 13]; // View as words.
	u32 swords[1 << 13]; // View as signed words.
} mem;
s32 regs[32]; // 32 registers

struct {
	int verbose;
	int readEcho;
} config = {0};

// Load the mem file into memory.
// example line "0: deadbeef"
int load_mem(const char *filename)
{
	FILE *fd;
	u32 offset, value;
	int res;

	memset(mem.bytes, 0, sizeof(mem.bytes));
	fd = fopen(filename);
	if (!fd)
		return -1;

	while(2 == (res = fscanf("%d: %lX", &offset, &value))) {
		mem.words[offset >> 3] = value; // Assign the value.
	}

	fclose(fd);

	return 0;
}

int main(int argc, char *argv[])
{
	// Parse arguments.
	for (int i = 1; i < argc; ++i) {
		char *arg = argv[i];
		if (arg[0] != '-') break;
		if (strequ(arg, "-v")) {
			config.verbose += 1;
		}
	}


}

