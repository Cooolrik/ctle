// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/bitmap_font.h"

using namespace ctle;

TEST( status , bitmap_font_test )
	{	
	uint8_t pix[64];

	bitmap_font<uint8_t> test_render;

	test_render.render( pix, 8, "A", 0xff );
	}