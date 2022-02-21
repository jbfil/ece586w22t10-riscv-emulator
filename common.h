

#define errorf(...) fprintf(stderr, __VA_ARGS__)
#define errorf_exit(...) do { fprintf(stderr, __VA_ARGS__); exit(-1); } while (0)

// String equality test.
int strequ(const char *a, const char *b);



