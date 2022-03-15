
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t	u08;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t	s32;

#define errorf(...) fprintf(stderr, __VA_ARGS__)
#define errorf_exit(...) do { fprintf(stderr, __VA_ARGS__); print_results(); exit(-1); } while (0)
#define detailsf(...) do { if(config.verbose >= 2 && config.show_details) { fprintf(stderr, __VA_ARGS__); }; } while (0)

// String equality test.
int strequ(const char *a, const char *b);



