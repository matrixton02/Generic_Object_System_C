#include "object.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


int main(){
    Object_t* intObj=new_object_Integer(5); //Stored an Inetger in out Object
    Object_t* intObj2=new_object_Integer(57); 
    Object_t* floatObj=new_object_Float(76.5f); //Stored a Float in out Object
    Object_t* strObj=new_object_String("Hello"); //Stored a string in our Object
    Object_t* c1=new_object_Complex(3.0f,4.0f); //Stored a Complex number in our Object
    Object_t* c2=new_object_Complex(5.0f,3.0f);
    Object_t* c3=add(c1,c2); // A new Object with complex number is generated using add function.
    Object_t* newStr=multiply(strObj,intObj); //Showcasing multiply fucntion with String and an Integer Object
    //Printing different objects values
    printf("%d\n",intObj->data.v_int);
    printf("%.2f\n",floatObj->data.v_float);
    printf("%s\n",strObj->data.v_string);
    printf("%s\n",newStr->data.v_string);
    printf("%.2f %.2f \n",c3->data.v_complex.real,c3->data.v_complex.imag);
    
    //Freeing all the objects
    object_free(intObj);
    object_free(intObj2);
    object_free(floatObj);
    object_free(strObj);
    object_free(c1);
    object_free(c2);
    object_free(c3);
    return 0;
}