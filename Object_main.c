#include "object.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


int main(){
    Object_t* intObj=new_object_Integer(5);
    Object_t* intObj2=new_object_Integer(57);
    Object_t* floatObj=new_object_Float(76.5f);
    char* s="hello";
    Object_t* strObj=new_object_String(s);
    Object_t* c1=new_object_Complex(3.0f,4.0f);
    Object_t* c2=new_object_Complex(5.0f,3.0f);
    Object_t* c3=add(c1,c2);
    printf("%.2f %.2f \n",c3->data.v_complex.real,c3->data.v_complex.imag);
    
    object_free(intObj);
    object_free(intObj2);
    object_free(floatObj);
    object_free(strObj);
    object_free(c1);
    object_free(c2);
    object_free(c3);
    return 0;
}