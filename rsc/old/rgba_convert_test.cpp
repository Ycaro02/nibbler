#include <assert.h>
#include <iostream>
#include "include/BasicDefine.hpp"

// Inclure les macros
#define RGBA_TO_UINT32(r, g, b, a) ((u32)((a << 24) | (r << 16) | (g << 8) | b))

#define UINT32_TO_RGBA(color, r, g, b, a) \
    r = (color >> 16) & 0xFF;             \
    g = (color >> 8) & 0xFF;              \
    b = color & 0xFF;                     \
    a = (color >> 24) & 0xFF;

typedef unsigned int u32;

void test_RGBA_TO_UINT32() {
    u32 color = RGBA_TO_UINT32(0, 0, 0, 0);
    assert(color == 0x00000000);

    color = RGBA_TO_UINT32(255, 255, 255, 255);
    assert(color == 0xFFFFFFFF);

    color = RGBA_TO_UINT32(1, 2, 3, 4);
    assert(color == 0x04010203);

    std::cout << "All RGBA_TO_UINT32 tests passed!" << std::endl;
}

void test_UINT32_TO_RGBA() {
    u32	color;
	u8	r, g, b, a;

    color = 0x00000000;
    UINT32_TO_RGBA(color, r, g, b, a);
    assert(r == 0);
    assert(g == 0);
    assert(b == 0);
    assert(a == 0);

    color = 0xFFFFFFFF;
    UINT32_TO_RGBA(color, r, g, b, a);
    assert(r == 255);
    assert(g == 255);
    assert(b == 255);
    assert(a == 255);

    color = 0x04010203;
    UINT32_TO_RGBA(color, r, g, b, a);
    assert(r == 1);
    assert(g == 2);
    assert(b == 3);
    assert(a == 4);




    std::cout << "All UINT32_TO_RGBA tests passed!" << std::endl;
}

void double_test() {
	u8 r,g,b,a;
	u32 color = RGBA_TO_UINT32(200, 100, 50, 255);
	UINT32_TO_RGBA(color, r, g, b, a);

	assert(r == 200);
	assert(g == 100);
	assert(b == 50);
	assert(a == 255);


	color = RGBA_TO_UINT32(0, 0, 0, 0);
	UINT32_TO_RGBA(color, r, g, b, a);
	assert (r == 0);
	assert (g == 0);
	assert (b == 0);
	assert (a == 0);


	color = RGBA_TO_UINT32(255, 255, 255, 255);
	UINT32_TO_RGBA(color, r, g, b, a);
	assert (r == 255);
	assert (g == 255);
	assert (b == 255);
	assert (a == 255);

	std::cout << "All double tests passed!" << std::endl;

}

int main() {
    test_RGBA_TO_UINT32();
    test_UINT32_TO_RGBA();
	double_test();
    return 0;
}