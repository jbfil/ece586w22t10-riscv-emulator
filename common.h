
#include <stdint.h>

typedef uint8_t	u08;
typedef uint32_t u32;
typedef int32_t	s32;

#define errorf(...) fprintf(stderr, __VA_ARGS__)
#define errorf_exit(...) do { fprintf(stderr, __VA_ARGS__); exit(-1); } while (0)

// String equality test.
int strequ(const char *a, const char *b);



