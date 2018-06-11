#ifndef smart_contract_output_h
#define smart_contract_output_h
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include "wren.hpp"


using namespace std;

// This module defines the built-in classes and their primitives methods that
// are implemented directly in C code. Some languages try to implement as much
// of the core module itself in the primary language instead of in the host
// language.
//
// With Wren, we try to do as much of it in C as possible. Primitive methods
// are always faster than code written in Wren, and it minimizes startup time
// since we don't have to parse, compile, and execute Wren code.
//
// There is one limitation, though. Methods written in C cannot call Wren ones.
// They can only be the top of the callstack, and immediately return. This
// makes it difficult to have primitive methods that rely on polymorphic
// behavior. For example, `IO.write` should call `toString` on its argument,
// including user-defined `toString` methods on user-defined classes.

int smart_contract_output( );
//int smart_contract_output(double &d1)//, double &d2, double &d3);
string run_wren_vm_when_activating_smart_contract(string input_source_code,string initargu);//首次激活合约时调用的接口函数 
string run_wren_vm_when_invoking_smart_contract(string input_source_code, string contranct_method, string input_data);//激活之后，合约每次被调用时，需要调用该函数
void Trim(char* str);


#endif
