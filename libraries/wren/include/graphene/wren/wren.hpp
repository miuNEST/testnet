#ifndef wren_hpp
#define wren_hpp

// This is a convenience header for users that want to compile Wren as C and
// link to it from a C++ application.

extern "C" {
//  #include "smart_contract_output.h"
/*  #include "wren_common.h"
  #include "wren_compiler.h"
  #include "wren_core.h"
  #include "wren_debug.h"
//  #include "wren_opcodes.h"
  #include "wren_opt_meta.h"
  #include "wren_opt_random.h"
  #include "wren_primitive.h"
  #include "wren_utils.h"
  #include "wren_value.h"
  #include "wren_vm.h" */
  #include "wren.h"
}
//#include "smart_contract_lifecycle.hpp"
#include "smart_contract_output.hpp"

#endif