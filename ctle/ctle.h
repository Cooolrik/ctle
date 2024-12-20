// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

// file which includes all of ctle, and can be used in a source file which defines CTLE_IMPLEMENTATION
// example usage:

/*

//
// example_implement.cpp - example implementation of ctle
//

// define CTLE_IMPLEMENTATION in this file only, this will include all the source code
#define CTLE_IMPLEMENTATION

// optionally, include headers of other libraries which may be used by ctle.
// including these will add more functionality to ctle
#include <vulkan/vulkan.h>	// convert vulkan errors to status errors
#include <system_error>		// convert system errors to status errors
#include <picosha2.h>		// hash calculation functions

// now, include ctle, which will implement the source code
#include <ctle/ctle.h>

*/

#include "bimap.h"
#include "bitmap_font.h"
#include "endianness.h"
#include "file_funcs.h"
#include "idx_vector.h"
#include "log.h"
#include "ntup.h"
#include "optional_idx_vector.h"
#include "optional_value.h"
#include "optional_vector.h"
#include "readers_writer_lock.h"
#include "prop.h"
#include "status.h"
#include "status_return.h"
#include "string_funcs.h"
#include "thread_safe_map.h"
#include "types.h"
#include "util.h"
#include "uuid.h"
#include "hash.h"
#include "sockets.h"
#include "read_stream.h"
#include "data_source.h"
#include "data_destination.h"
#include "hasher.h"