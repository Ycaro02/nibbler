#include <stdio.h>
#include <assert.h>

/* Macro to set bit_nb in buff to val */
#define U32_SET_BIT(buff, bit_nb, val) \
    ((buff) = ((buff) & ~(1 << (bit_nb))) | ((val) << (bit_nb)))

/* Macro to get bit_nb in buff */
#define U32_GET_BIT(buff, bit_nb) (((buff) >> (bit_nb)) & 1)

typedef unsigned int u32;

int main (void) {
	u32 buffer = 0xffffffff;
	U32_SET_BIT(buffer, 0, 1);
	assert(U32_GET_BIT(buffer, 0) == 1);

	U32_SET_BIT(buffer, 1, 1);
	assert(U32_GET_BIT(buffer, 1) == 1);

	U32_SET_BIT(buffer, 2, 0);
	assert(U32_GET_BIT(buffer, 2) == 0);

	for (int i = 0; i < 32; i++) {
		u32 test_val = i % 2;
		U32_SET_BIT(buffer, i, test_val);
		assert(U32_GET_BIT(buffer, i) == test_val);
	}

	printf("All tests passed\n");
	return 0;
}