#include "testclass.h"

TestClass::TestClass()
{
    i = 0;
}

void TestClass::increment(){
    i++;
}

int TestClass::geti(){
    return i;
}
