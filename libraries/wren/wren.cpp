//#include <wren.h>
#include <stdio.h>
#include <iostream>
#include "wren.hpp"
#include "smart_contract_output.hpp"

using namespace std;

int wren()
{
	double d1,d2,d3;

//	smart_contract_output(d1, d2, d3);
//	WrenConfiguration config; 
//	wrenInitConfiguration(&config);
//	WrenVM* vm = wrenNewVM(&config);

// 	WrenInterpretResult result = wrenInterpret(vm, "System.print(\"I am running in a VM!\")");
//	wrenFreeVM(vm);

	std::cout<<"d1 = "<<d1<<"\n d2 = "<<d2<<"\n d3 = "<<d3<<std::endl;
	
	return 0;
}
