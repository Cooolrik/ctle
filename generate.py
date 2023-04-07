# This file generates needed ctle code using the code generator module
import ctle_code_gen

print('ctle code generation running...')

# generate status.h 
ctle_code_gen.status_generator.generate_status('./ctle/status.h')
