// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/bitmap_font.h"

#include "unit_tests.h"

using namespace ctle;

static constexpr const int test_buffer_w = 60;
static constexpr const int test_buffer_h = 17;

static std::string write_out_text( uint8_t *pix )
{
	std::string out;
	for( int y = 0; y < test_buffer_h; ++y )
	{
		for( int x = 0; x < test_buffer_w; ++x )
		{
			if( pix[x + ( y * test_buffer_w )] > 0 )
				out += '*';
			else
				out += ' ';
		}
	}
	return out;
}

static const char *expected_text_regular =
	"                                                            "
	"                                                            "
	"**   **             ***      ***                            "
	"**   **              **       **                            "
	"**   **              **       **                            "
	"**   **   *****      **       **     *****                  "
	"*******  **   **     **       **    **   **                 "
	"**   **  *******     **       **    **   **                 "
	"**   **  **          **       **    **   **                 "
	"**   **  **          **       **    **   **                 "
	"**   **  **   **     **       **    **   **                 "
	"**   **   *****     ****     ****    *****                  "
	"                                                            "
	"                                                            "
	"                                                            "
	"                                                            "
	"                                                            ";

TEST( bitmap_font, regular_test )
{
	uint8_t pix[test_buffer_w * test_buffer_h] = {};

	render_bitmap_font<uint8_t>( pix, test_buffer_w, 0, 0, "Hello", 0xff );

	EXPECT_TRUE( write_out_text( pix ) == expected_text_regular );
}

static const char *expected_text_flipped =
	"                                                            "
	"                                                            "
	"                                                            "
	"                                                            "
	"                                                            "
	"        **   **   *****     ****    *** **     **           "
	"        **   **  **   **     **    **  **      **           "
	"        **   **  **   **     **    **  **                   "
	"        **   **  **   **     **    **  **      **           "
	"        **   **  **   **     **     *****      **           "
	"        *******  **   **     **        **      **           "
	"        **   **   *****      **     ****      ****          "
	"        **   **              **               ****          "
	"        **   **              **               ****          "
	"        **   **             ***                **           "
	"                                                            "
	"                                                            ";

TEST( bitmap_font, flipped_test )
{
	uint8_t pix[test_buffer_w * test_buffer_h] = {};

	// place the position in the center of ther output buffer
	int xpos = test_buffer_w / 2;
	int ypos = 8;

	render_bitmap_font<uint8_t>(
		pix,
		test_buffer_w,
		xpos,
		ypos,
		"Hola!",
		0xff,
		{},
		bitmap_font_flags::center_horizontally | bitmap_font_flags::center_vertically | bitmap_font_flags::flip_y
	);

	EXPECT_TRUE( write_out_text( pix ) == expected_text_flipped );
}
