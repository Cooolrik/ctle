# This file generates needed ctle code using the code generator module
import ctle_code_gen

print('ctle code generation running...')

# generate property.h 
ctle_code_gen.property_generator.generate_property('./ctle/prop.h')

# generate status.h 
ctle_code_gen.status_generator.generate_status('./ctle/status.h')

# generate types.h
ctle_code_gen.types_generator.generate_types('./ctle/types.h')

# generate _macros.inl and _undef_macros.inl
ctle_code_gen.macros_generator.generate_macros('./ctle/_macros.inl', './ctle/_undef_macros.inl')