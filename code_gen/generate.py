# ctle Copyright (c) 2022 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

# This file generates needed ctle code using the code generator module
import property_generator
import status_generator
import types_generator
import macros_generator

print('ctle code generation running...')

# generate property.h 
property_generator.generate_property('../include/ctle/prop.h')

# generate status.h 
status_generator.generate_status('../include/ctle/status.h')

# generate types
types_generator.generate_types('../include/ctle', '../unit_tests')

# generate _macros.inl and _undef_macros.inl
macros_generator.generate_macros('../include/ctle/_macros.inl', '../include/ctle/_undef_macros.inl')