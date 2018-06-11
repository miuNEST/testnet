#include "smart_contract_output.hpp"
#include "wren.h"
#include "wren.hpp"


static WrenHandle* handleObj;

typedef enum {
  UNACTIVE,
  ACTIVE,
  DEAD
} SmartContractObjectState;

void reportError(WrenVM* vm, WrenErrorType type,
                        const char* module, int line, const char* message)
{
  switch (type)
  {
    case WREN_ERROR_COMPILE:
      fprintf(stderr, "[%s line %d] %s\n", module, line, message);
      break;
      
    case WREN_ERROR_RUNTIME:
      fprintf(stderr, "%s\n", message);
      break;
      
    case WREN_ERROR_STACK_TRACE:
      fprintf(stderr, "[%s line %d] in %s\n", module, line, message);
      break;
  }
}
/*
class smart_contract_obj_in_wren
{
}

class smart_contract_obj_in_cpp
{
  Address addr_of_obj;
  SmartContractObjectState = UNACTIVE;
  string *sourceCode = NULL; 
//  smart_contract_obj_in_wren *obj = NULL;//这里预留用户存储从wren里抓出来的对象（的数据部分）
}


void addressAllocate(WrenVM* vm) 
{
  const char* addr_string_in_c = wrenGetSlotString(vm, 1);
  int temp = strlen(addr_string_in_c);
  char* const address_id = (char*)wrenSetSlotNewForeign( vm, 0, 0, temp );
  for (int i = 0; i < temp; i++)
  {
      address_id[i] = addr_string_in_c[i];
      cout<<address_id[i];  
  }
  cout<<endl;

  cout<<"wrenGetSlotType(vm, 0) = "<<wrenGetSlotType(vm, 0)<<endl;
  handleObj = wrenGetSlotHandle(vm, 0);
  cout<<"handle of slot 0 in constructor function is "<<handleObj<<endl;
}

void addressFinalize(void* data) //这个需要重新写
{  
}*/
//============================================================
void implement_delegatedCall(WrenVM * vm)
{
  cout<<"hello world!"<<endl;
  wrenSetSlotString(vm, 0,"{\"a\":1,\"b\":\"2\"}");
  
}
char out[2048]={0x00};
int len =0;
void implement_wrenToString(WrenVM * vm)
{
 // cout<<"implement_wrenToString"<<endl;
 // cout<<"+++++++++++7777777+++++++++"<<endl;
 // cout<<"wrenGetSlotType(vm, 1) = "<<wrenGetSlotType(vm, 1)<<endl;
  if(len >3)
    len =0;
  char temp[128]={0x00};
  bool b_temp;
  double d_temp;
   char* s_temp; 
  WrenType slot1_Type = wrenGetSlotType(vm, 1);
  switch (slot1_Type)
  {
    case WREN_TYPE_BOOL:
    {
      b_temp = wrenGetSlotBool(vm,1);
   //   cout<<"WREN_TYPE_BOOL "<<b_temp<<endl;
      if (b_temp)
        sprintf(temp,"obj.set_state_%d(true)",len);
      else
        sprintf(temp,"obj.set_state_%d(false)",len);
      len +=1;
      wrenSetSlotDouble(vm,0,1);
      break;
    }
    case WREN_TYPE_NUM:
    {
      d_temp = wrenGetSlotDouble(vm,1);
     // cout<<"WREN_TYPE_NUM "<<d_temp<<endl;
      int d =d_temp;
      sprintf(temp,"obj.set_state_%d(%d)",len,d);
      len+=1;
      wrenSetSlotDouble(vm,0,2);
      break;
    }
    case WREN_TYPE_STRING:
    {
      s_temp = (char *)wrenGetSlotString(vm,1);
      if (s_temp[0]=='['  ){
         int a = strlen(s_temp);
         s_temp[a-1]=']';
         cout<<s_temp<<endl;
      }
      if (s_temp[0]=='{'){
        int b = strlen(s_temp);
         s_temp[b-1]='}';
         cout<<s_temp<<endl;
      }
     // cout<<"WREN_TYPE_STRING "<<s_temp<<endl;
     if (s_temp[0]=='[' || s_temp[0]=='{' ){
       sprintf(temp,"obj.set_state_%d(%s)",len,s_temp);
       cout<<temp<<endl;
     }else{
       sprintf(temp,"obj.set_state_%d(\"%s\")",len,s_temp);
     }
      
      len +=1;
      wrenSetSlotDouble(vm,0,3);
      break;
    }
    default: 
    {
      wrenSetSlotDouble(vm,0,4);
      break;
    }
    
  }
  
}

void implement_mapToGettype(WrenVM * vm)
{
  char* s_temp; 

  s_temp = (char *)wrenGetSlotString(vm,1);
  if (s_temp[0]!='\"' ){
      if(memcmp(s_temp,"false",5)==0 )
        wrenSetSlotBool(vm,0,false);
      else if(memcmp(s_temp,"true",4)==0)
        wrenSetSlotBool(vm,0,true);
      else{
        wrenSetSlotDouble(vm,0,3);
      }
  }else{
      wrenSetSlotDouble(vm,0,4);
  }
  
}

void implement_mapToGettype_ex(WrenVM * vm)
{
  char* s_temp; 

  s_temp = (char *)wrenGetSlotString(vm,1);
  if (s_temp[0]!='\"' ){
      if(memcmp(s_temp,"false",5)==0 )
        wrenSetSlotBool(vm,0,false);
      else if(memcmp(s_temp,"true",4)==0)
        wrenSetSlotBool(vm,0,true);
      else{
        wrenSetSlotDouble(vm,0,3);
      }
  }else{
      wrenSetSlotDouble(vm,0,4);
  }
  
}

char* state;
void implement_setStateToString(WrenVM * vm)
{
  state = (char *)wrenGetSlotString(vm,1);
}

void implement_getStateFromString(WrenVM * vm)
{
  wrenSetSlotString(vm, 0,state);
}

void implement_getStateFromString_ex(WrenVM * vm)
{
  wrenSetSlotString(vm, 0,state);
}
//============================================================
void implement_saveObj(WrenVM * vm)
{

  cout<<"saveing object!"<<endl;
  cout<<"wrenGetSlotType(vm, 0) = "<<wrenGetSlotType(vm, 0)<<endl;
  cout<<"wrenGetSlotCount(vm) = "<<wrenGetSlotCount(vm)<<endl;

  cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
  WrenHandle *method_temp = wrenMakeCallHandle(vm, "getMemberVariable()");
  const char * str = wrenGetSlotString(vm, 0);
  cout<<"&&&)))))))))))))))))))((((((((((((((&&&&&&&"<<endl;
  cout<<str[0]<<str[1]<<str[2]<<str[3]<<str[4]<<str[5]<<endl;
//  handleObj = wrenGetSlotHandle(vm, 0);
//  WrenHandle* handleObj;
//  cout<<"wrenGetSlotType(vm, 1) = "<<wrenGetSlotType(vm, 1)<<endl;
//  cout<<"handle of slot 0 in saveObj function is "<<handleObj<<endl;
 // cout<<"complete saveing object!"<<endl;
}

void implement_unsaveObj(WrenVM * vm)
{
  cout<<"unsaveing object! That means that a object instant is reimplemented into wren"<<endl;
//  wrenSetSlotHandle(vm, 0, handleObj);
  cout<<"complete unsaveing object!"<<endl;
}

//============================================================
void implement_activateSmartContractObjInCpp(WrenVM * vm)
{
  //*****************************************
}
//============================================================
static void write(WrenVM* vm, const char* text)
{
  cout<<text<<endl;
}

//============================================================


/*
WrenForeignClassMethods bindForeignClass_selfDef(WrenVM* vm, const char* module, const char* className) 
{ 
  WrenForeignClassMethods methods;

  if (strcmp(className, "Address") == 0) 
  { 
    methods.allocate = addressAllocate; 
    methods.finalize = addressFinalize; 
  } 
//  else if (strcmp(className, "Interface") == 0)
//  {
//    methods.allocate = interfaceAllocate; 
//    methods.finalize = interfaceFinalize;
//  }
  else 
  { 
    // Unknown class.
    methods.allocate = NULL; 
    methods.finalize = NULL; 
  }

  return methods; 
}
*/

WrenForeignMethodFn bindForeignMethod_selfDef(WrenVM* vm, const char* module, 
    const char* className, bool isStatic, const char* signature) 
{ 
  if (strcmp(className, "Address") == 0) 
  { 
    if (!isStatic && strcmp(signature, "delegateCall()") == 0) 
    { 
      return implement_delegatedCall; 
    }
    else if (!isStatic && strcmp(signature, "saveObj()") == 0)
    {
      return implement_saveObj;
    }
    else if (!isStatic && strcmp(signature, "unsaveObj()") == 0)
    {
      return implement_unsaveObj;
    }
  }

  if (strcmp(className, "Interface") == 0) 
  { 
    if (!isStatic && strcmp(signature, "activateSmartContractObjInCpp()") == 0)///////////////// 
    { 
      return implement_activateSmartContractObjInCpp; 
    } 
  }

  if (strcmp(className, "A") == 0)
  {
    if (!isStatic && strcmp(signature, "wrenToString(_)") == 0)///////////////// 
    { 
      return implement_wrenToString; 
    }
    else if(!isStatic && strcmp(signature, "mapToGettype(_)") == 0)
    {
      return implement_mapToGettype;
    } 
  }

  if (strcmp(className, "Contract") == 0)
  {
    if (!isStatic && strcmp(signature, "wrenToString(_)") == 0)///////////////// 
    { 
      return implement_wrenToString; 
    }
    else if(!isStatic && strcmp(signature, "mapToGettype(_)") == 0)
    {
      return implement_mapToGettype;
    }
    else if(/*!isStatic && */strcmp(signature, "mapToGettype_ex(_)") == 0)
    {
      return implement_mapToGettype_ex;
    }
    else if(!isStatic && strcmp(signature, "setStateToString(_)") == 0)
    {
      return implement_setStateToString;
    }
    else if(!isStatic && strcmp(signature, "getStateFromString()") == 0)
    {
      return implement_getStateFromString;
    }
     else if(/*!isStatic && */strcmp(signature, "getStateFromString_ex()") == 0)
    {
      return implement_getStateFromString_ex;
    }

  }
  return NULL; 

}

static char const* rootDirectory = NULL;

static char* readFile(const char* path)
{
  FILE* file = fopen(path, "rb");
  if (file == NULL) return NULL;
  
  // Find out how big the file is.
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);
  
  // Allocate a buffer for it.
  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }
  
  // Read the entire file.
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }
  
  // Terminate the string.
  buffer[bytesRead] = '\0';
  
  fclose(file);
  return buffer;
}

static char* wrenFilePath(const char* name)
{
  // The module path is relative to the root directory and with ".wren".
  size_t rootLength = rootDirectory == NULL ? 0 : strlen(rootDirectory);
  size_t nameLength = strlen(name);
  size_t pathLength = rootLength + nameLength + 5;
  char* path = (char*)malloc(pathLength + 1);
  
  if (rootDirectory != NULL)
  {
    memcpy(path, rootDirectory, rootLength);
  }
  
  memcpy(path + rootLength, name, nameLength);
  memcpy(path + rootLength + nameLength, ".wren", 5);
  path[pathLength] = '\0';
  
  return path;
}

static char* readModule(WrenVM* vm, const char* module)
{
 // char* source = readBuiltInModule(module);
//  if (source != NULL) return source;
  
  // First try to load the module with a ".wren" extension.
  char* modulePath = wrenFilePath(module);
  char* moduleContents = readFile(modulePath);
  free(modulePath);
  
  if (moduleContents != NULL) return moduleContents;
  
  // If no contents could be loaded treat the module name as specifying a
  // directory and try to load the "module.wren" file in the directory.
  size_t moduleLength = strlen(module);
  size_t moduleDirLength = moduleLength + 7;
  char* moduleDir = (char*)malloc(moduleDirLength + 1);
  memcpy(moduleDir, module, moduleLength);
  memcpy(moduleDir + moduleLength, "/module", 7);
  moduleDir[moduleDirLength] = '\0';
  
  char* moduleDirPath = wrenFilePath(moduleDir);
  free(moduleDir);
  
  moduleContents = readFile(moduleDirPath);
  free(moduleDirPath);
  
  return moduleContents;
}

WrenVM* vm1;
   

int smart_contract_output()
{
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    
//  config.bindForeignClassFn = bindForeignClass_selfDef;
    config.bindForeignMethodFn = bindForeignMethod_selfDef;
    config.writeFn = write;
    config.errorFn = reportError;
    config.initialHeapSize = 1024 * 1024 * 10;
    config.loadModuleFn = readModule;

    vm1 = wrenNewVM(&config);

    
  
//  printf("%p\n", vm);
/*====================================
    FILE* fp;
    char* data;
    fp = fopen("abc.wren","r");
    if(fp == NULL)
    {
        perror("Open file recfile");
        exit(1);
    }
    fseek(fp,0,SEEK_END);
    int length = ftell(fp);
    data = (char *)malloc((length + 1) * sizeof(char));
    rewind(fp);
    length = fread(data, 1, length, fp);
    data[length] = '\0';

    WrenInterpretResult result = wrenInterpret(vm, data);

 //   printf("result: %d\n", result);
    fclose(fp);
    free(data);
//*/ //====================================
/*====================================
    FILE* fp1;
    char* data1;
    fp1 = fopen("b001.wren","r");
    if(fp1 == NULL)
    {
        perror("Open file recfile");classname
        exit(1);
    }
    fseek(fp1,0,SEEK_END);
    int length1 = ftell(fp1);
    data1 = (char *)malloc((length1 + 1) * sizeof(char));
    rewind(fp1);
    length1 = fread(data1, 1, length1, fp1);
    data1[length1] = '\0';
    printf("B smartcontract is called\n");

    WrenInterpretResult result1 = wrenInterpret(vm, data1);

    printf("result1: %d\n", result1);
    fclose(fp1);
    free(data1);
//*/ //====================================
  
    //string a ="123";
    //  char * b =(char *)a.c_str();
    //  cout<< b <<endl;  
    // return 0;
    
    //test 
    string demo = "class A is Contract {  \n"
    "construct new(lis) {     //must be lists \n"
    "    _p1 = lis[0] \n"
    "    _p2 = lis[1]\n"
    "    _p3 = lis[2]\n"
    "}\n"
    "member_variables{[_p1, _p2, _p3]} //must be assign  \n"
    "member1_add(p) {_p1 = _p1 + p} \n"
    "member2_add(p) {_p2 = _p2 + p} \n"
    "member3_add(p) {_p3 = _p3 + p} \n"
    "add(a,b){       \n"
    "    return a+b   \n"
    "}    \n"
   "}";
   string state = "100,false,\"bob\"";
   string out =run_wren_vm_when_activating_smart_contract(demo,state);
   string method ="member1_add(20)";
   out =run_wren_vm_when_invoking_smart_contract(demo, method, "");
   
   wrenFreeVM(vm1);
    
    return 0;

}

void Trim(char* str) 
{
	char *ptail, *p;
	p = str;
	while(*p && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n')) p++;//\n是换行，光标下移一格;\r是回车，使光标到行首；\t是Tab键;
	if (!*p) 
	{
		*str = 0;
		return ;
	}
	
	ptail = p+strlen(p)-1;
	while(ptail > p && (*ptail == ' ' || *ptail == '\t' || *ptail == '\n' || *ptail == '\r')) 
	{
		*ptail = 0;
		ptail--;
	}
	
	int L = strlen(p);
	memmove(str, p, L);
	str[L] = 0;
}

string run_wren_vm_when_activating_smart_contract(string input_source_code,string initargu)//首次激活合约时调用的接口函数 
{
  //add for test
    WrenVM* vm;
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.bindForeignMethodFn = bindForeignMethod_selfDef;
    config.writeFn = write;
    config.errorFn = reportError;
    config.initialHeapSize = 1024 * 1024 * 10;
    config.loadModuleFn = readModule;
    vm = wrenNewVM(&config);
  //
     string temp = input_source_code;
     int fclass1 =temp.find("class ");
     int fclass2 =temp.find("is",fclass1+6);
     string classname = temp.substr(fclass1+6,fclass2-fclass1-6);
     int fconstruct1 =temp.find("construct ");
     int fconstruct2 =temp.find("(",fconstruct1+10);
     string constructname =temp.substr(fconstruct1+10,fconstruct2-fconstruct1-10);
     Trim((char *)classname.c_str());
     Trim((char *)constructname.c_str());
     string init = (char *)classname.c_str();
     init +="."+constructname+"([";
     init +=initargu;
     init +="])";
     string code_pre ="import \"contract\" for Contract\n";
     code_pre += input_source_code;
     code_pre += "\nvar obj =";
     code_pre += init;
     code_pre += "\nobj.save_member_variables_into_c()\n";
     char* data=(char *)code_pre.c_str();
     cout<< data <<endl;
     WrenInterpretResult result = wrenInterpret(vm,data);
     wrenFreeVM(vm);
     string output;
     if(result != 0)
     {
       if(result==1)
          output="error:WREN_RESULT_COMPILE_ERROR";
       else
          output="error:WREN_RESULT_RUNTIME_ERROR";
       cout<<"output = "<<output<<endl;
       return output;
     }
     output = state;
     cout<<"output = "<<output<<endl;
     return output; 
}  
string run_wren_vm_when_invoking_smart_contract(string input_source_code, string contranct_method_and_parameter, string starting_state)//激活之后，合约每次被调用时，需要调用该函数
{
  //add for test
    WrenVM* vm;
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.bindForeignMethodFn = bindForeignMethod_selfDef;
    config.writeFn = write;
    config.errorFn = reportError;
    config.initialHeapSize = 1024 * 1024 * 10;
    config.loadModuleFn = readModule;
    vm = wrenNewVM(&config);
  //
  string temp = input_source_code;
  int fclass1 =temp.find("class ");
  int fclass2 =temp.find("is",fclass1+6);
  string classname = temp.substr(fclass1+6,fclass2-fclass1-6);
  int fconstruct1 =temp.find("construct ");
  int fconstruct2 =temp.find("(",fconstruct1+10);
  string constructname =temp.substr(fconstruct1+10,fconstruct2-fconstruct1-10);
  Trim((char *)classname.c_str());
  Trim((char *)constructname.c_str());
  string init = (char *)classname.c_str();
 // init +="."+constructname+"(Contract.reassemble_state())\n";
  init +="."+constructname+"("+starting_state+")\n";
  string code_pre ="import \"contract\" for Contract\n";
  code_pre += input_source_code;
  code_pre += "\nvar obj1 =";
  code_pre += init;
  code_pre += "obj1.";
  code_pre += contranct_method_and_parameter;
  code_pre += "\nobj1.save_member_variables_into_c()\n";
  char* data=(char *)code_pre.c_str();
  cout<< "data = " << data <<endl;
  cout<<"starting_state "<<starting_state<<endl;
  WrenInterpretResult result = wrenInterpret(vm,data);
  wrenFreeVM(vm);
  string output ;
  if(result != 0)
  {
      if(result==1)
          output="error:WREN_RESULT_COMPILE_ERROR";
      else
          output="error:WREN_RESULT_RUNTIME_ERROR";
      return output;
  }
  output = state;
  return output;  

}