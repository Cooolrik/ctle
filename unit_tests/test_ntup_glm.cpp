// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/ntup.h>
#include <ctle/ntup_glm.h>

#include "unit_tests.h"

using namespace ctle;

static_assert( !std::is_constructible<n_tup<float, 3>, int>::value, "scalar conversion must remain disabled" );
static_assert( !std::is_assignable<n_tup<float, 3> &, std::string>::value, "unregistered types must not assign" );
static_assert( !std::is_convertible<glm::dvec3, n_tup<float, 3>>::value, "scalar changes must remain explicit" );
static_assert( !std::is_convertible<n_tup<float, 3>, glm::dvec3>::value, "outward scalar changes must stay disabled" );
static_assert( !std::is_convertible<glm::vec4, n_tup<float, 3>>::value, "dimension changes must remain explicit" );
static_assert( std::is_trivially_copyable<mn_tup<float, 2, 3>>::value, "matrix tuples must remain trivially copyable" );

TEST( ntup_glm, late_vector_adapter )
{
	n_tup<float, 3> tuple( glm::vec4( 1, 2, 3, 4 ) );
	glm::vec3 converted = tuple;
	EXPECT_EQ( converted, glm::vec3( 1, 2, 3 ) );

	tuple = glm::dvec3( 1.25, 2.5, 3.75 );
	converted = tuple;
	EXPECT_EQ( converted, glm::vec3( 1.25f, 2.5f, 3.75f ) );

	const glm::vec<3, float, glm::packed_lowp> qualified( 4, 5, 6 );
	n_tup<float, 3> from_qualified( qualified );
	tuple = qualified;
	EXPECT_EQ( tuple, from_qualified );
	converted = tuple;
	EXPECT_EQ( converted, glm::vec3( 4, 5, 6 ) );
}

TEST( ntup_glm, late_matrix_adapter )
{
	const glm::mat3x2 original( glm::vec2( 1, 2 ), glm::vec2( 3, 4 ), glm::vec2( 5, 6 ) );
	mn_tup<float, 2, 3> tuple = original;
	glm::mat3x2 converted = tuple;
	EXPECT_EQ( converted, original );
	EXPECT_EQ( tuple.values[2].y, 6 );

	const glm::mat2x3 other( glm::vec3( 7, 8, 9 ), glm::vec3( 10, 11, 12 ) );
	const glm::mat3x2 expected = other;
	mn_tup<float, 2, 3> from_other( other );
	tuple = other;
	converted = tuple;
	EXPECT_EQ( tuple, from_other );
	EXPECT_EQ( converted, expected );

	const glm::dmat3x2 precise( glm::dvec2( 1.25, 2.5 ), glm::dvec2( 3.75, 4 ),
		glm::dvec2( 5, 6 ) );
	tuple = precise;
	converted = tuple;
	EXPECT_EQ( converted, glm::mat3x2( precise ) );
}
