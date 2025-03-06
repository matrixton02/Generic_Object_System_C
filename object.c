#include "object.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

Object_t* new_Object(){
    Object_t* obj=malloc(sizeof(Object_t));
    if(obj==NULL){
        return NULL;
    }
    obj->refcount=1;
    return obj; 
}

void refcount_inc(Object_t* obj){
    if(obj==NULL){
        return;
    }
    obj->refcount++;
}

void refcount_dec(Object_t* obj){
    if(obj==NULL){
        return;
    }
    obj->refcount--;
    if(obj->refcount==0){
        object_free(obj);
    }
}

void object_free(Object_t* obj){
    if(obj==NULL){
        return; 
    }
    switch(obj->kind){
        case INTEGER:
            break;
        case FLOAT:
            break;
        case COMPLEX:
            break;
        case VECTOR:
            refcount_dec(obj->data.v_vector.x);
            refcount_dec(obj->data.v_vector.y);
            refcount_dec(obj->data.v_vector.z);
        case STRING:
            free(obj->data.v_string);
            break;
        case ARRAY:
            
            for(int i=0;i<obj->data.v_array.size;i++){
                refcount_dec(obj->data.v_array.elements[i]);
            }
            free(obj->data.v_array.elements);
            break;
        default:
            return;
    }
    free(obj);
}

Object_t* new_object_Integer(int value){
    Object_t *obj=new_Object();

    if(obj==NULL){
        return NULL;
    }
    obj->kind=INTEGER;
    obj->data.v_int=value;

    return obj;
}

Object_t* new_object_Float(float value){
    Object_t *obj=new_Object();

    if(obj==NULL){
        return NULL;
    }

    obj->kind=FLOAT;
    obj->data.v_float=value;

    return obj;
}

Object_t* new_object_String(char* value){
    Object_t* obj=new_Object();
    if(obj==NULL){
        return NULL;
    }
    obj->kind=STRING;
    obj->data.v_string=malloc(strlen(value)+1);
    if(obj->data.v_string==NULL){
        return NULL;
    }

    strcpy(obj->data.v_string,value);
    return obj;
}

Object_t* new_object_Vector(Object_t* x,Object_t* y,Object_t* z){
    if(x==NULL || y==NULL || z==NULL){
        return NULL;
    }

    Object_t* obj=new_Object();
    if(obj==NULL){
        return NULL;
    }
    obj->kind=VECTOR;
    refcount_inc(x);
    refcount_inc(y);
    refcount_inc(z);
    obj->data.v_vector.x=x;
    obj->data.v_vector.y=y;
    obj->data.v_vector.z=z;
    return obj;
}

Object_t* new_object_Array(int size){
    Object_t* obj=new_Object();

    if(obj==NULL){
        return NULL;
    }
    obj->kind=ARRAY;
    Object_t **elements=calloc(sizeof(Object_t *),size);
    if(elements==NULL){
        return NULL;    
    }
    Object_Array_t array={.size=size,.elements=elements};
    obj->data.v_array=array;
    return obj;
}

int array_set(Object_t* obj,int index,Object_t* value){
    if(obj==NULL || value==NULL){
        return 0;
    }
    if(obj->kind!=ARRAY){
        return 0;
    }
    if(obj->data.v_array.size<=index){
        return 0;
    }
    refcount_inc(value);
    if(obj->data.v_array.elements[index]!=NULL){
        refcount_dec(obj->data.v_array.elements[index]);
    }
    obj->data.v_array.elements[index]=value;
    return 1;
}

Object_t* array_get(Object_t* obj,int index){
    if(obj==NULL){
        return NULL;
    }
    if(obj->kind!=ARRAY){
        return NULL;
    }
    if(obj->data.v_array.size<=index){
        return NULL;
    }
    return obj->data.v_array.elements[index];
}

Object_t* new_object_Complex(float a,float b){
    Object_t* obj=new_Object();

    if(obj==NULL){
        return NULL;
    }
    obj->kind=COMPLEX;
    obj->data.v_complex.real=a;
    obj->data.v_complex.imag=b;

    return obj;
}

int object_lenght(Object_t* obj){
    if(obj==NULL){
        return -1;
    }

    switch(obj->kind){
        case INTEGER:
            return 1;
        case FLOAT:
            return 1;
        case STRING:
            return strlen(obj->data.v_string);
        case VECTOR:
            return 3;
        case ARRAY:
            return obj->data.v_array.size;
        case COMPLEX:
            return 2;
        default:
            return -1;
    }
}

Object_t* add(Object_t* a,Object_t* b){
    if(a==NULL || b==NULL){
        return NULL;
    }

    switch(a->kind){
        case INTEGER: {
            switch(b->kind){
                case INTEGER:
                    return new_object_Integer(a->data.v_int+b->data.v_int);
                case FLOAT:
                    return new_object_Float((float)a->data.v_int+b->data.v_float);
                default:
                    return NULL;
            }
        }
        case FLOAT: {
            switch(b->kind){
                case INTEGER:
                    return new_object_Float(a->data.v_float+(float)b->data.v_int);
                case FLOAT:
                    return new_object_Float(a->data.v_float+b->data.v_float);
                default:
                    return NULL;
            }
        }
        case STRING:{
            if(b->kind!=STRING){
                return NULL;
            }
            int length=strlen(a->data.v_string)+strlen(b->data.v_string)+1;
            char* newString=calloc(sizeof(char*),length);
            strcat(newString,a->data.v_string);
            strcat(newString,b->data.v_string);
            Object_t* obj=new_object_String(newString);
            free(newString);
            return obj;
        }
        case COMPLEX:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Complex(a->data.v_complex.real+b->data.v_int,a->data.v_complex.imag);
                case FLOAT:
                    return new_object_Complex(a->data.v_complex.real+b->data.v_int,a->data.v_complex.imag);
                case COMPLEX:
                    return new_object_Complex(a->data.v_complex.real+b->data.v_complex.real,a->data.v_complex.imag+b->data.v_complex.imag);
                default:
                    return NULL;
            }
        }
        case VECTOR:{
            if(b->kind!=VECTOR){
                return NULL;
            }
            return new_object_Vector(
                add(a->data.v_vector.x,b->data.v_vector.x),
                add(a->data.v_vector.y,b->data.v_vector.y),
                add(a->data.v_vector.z,b->data.v_vector.z)               
            );
        }
        case ARRAY: {
            if(b->kind!=ARRAY){
                return NULL;
            }
            int lenght=a->data.v_array.size+b->data.v_array.size;
            Object_t* obj=new_object_Array(lenght);
            for(int i=0;i<a->data.v_array.size;i++){
                array_set(obj,i,array_get(a,i));
            }
            for(int i=0;i<b->data.v_array.size;i++){
                array_set(obj,i+a->data.v_array.size,array_get(b,i));
            }
            return obj;
        }
        default:
            return NULL;
    }
}

Object_t* sub(Object_t* a,Object_t* b){
    if(a==NULL || b==NULL){
        return NULL;
    }

    switch(a->kind){
        case INTEGER: {
            switch(b->kind){
                case INTEGER:
                    return new_object_Integer(a->data.v_int-b->data.v_int);
                case FLOAT:
                    return new_object_Float((float)a->data.v_int-b->data.v_float);
                default:
                    return NULL;
            }
        }
        case FLOAT:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Float(a->data.v_float-(float)b->data.v_int);
                case FLOAT:
                    return new_object_Float(a->data.v_float-b->data.v_float);
                default:
                    return NULL;
            }
        }
        case COMPLEX:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Complex(a->data.v_complex.real-b->data.v_int,a->data.v_complex.imag);
                case FLOAT:
                    return new_object_Complex(a->data.v_complex.real-b->data.v_int,a->data.v_complex.imag);
                case COMPLEX:
                    return new_object_Complex(a->data.v_complex.real-b->data.v_complex.real,a->data.v_complex.imag-b->data.v_complex.imag);
                default:
                    return NULL;
            }
        }
        case VECTOR:{
            if(b->kind!=VECTOR){
                return NULL;
            }
            return new_object_Vector(
                sub(a->data.v_vector.x,b->data.v_vector.x),
                sub(a->data.v_vector.y,b->data.v_vector.y),
                sub(a->data.v_vector.z,b->data.v_vector.z)               
            );
        }
        default:
            return NULL;
    }
}

Object_t* multiply(Object_t* a,Object_t* b){
    if(a==NULL || b==NULL){
        return NULL;
    }

    switch(a->kind){
        case INTEGER: {
            switch(b->kind){
                case INTEGER:
                    return new_object_Integer(a->data.v_int*b->data.v_int);
                case FLOAT:
                    return new_object_Float((float)a->data.v_int*b->data.v_float);
                default:
                    return NULL;
            }
        }
        case FLOAT:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Float(a->data.v_float*(float)b->data.v_int);
                case FLOAT:
                    return new_object_Float(a->data.v_float*b->data.v_float);
                default:
                    return NULL;
            }
        }
        case VECTOR:{
            if(b->kind!=VECTOR){
                return NULL;
            }
            return new_object_Vector(
                multiply(a->data.v_vector.x,b->data.v_vector.x),
                multiply(a->data.v_vector.y,b->data.v_vector.y),
                multiply(a->data.v_vector.z,b->data.v_vector.z)               
            );
        }
        case STRING:{
            if(b->kind!=INTEGER){
                return NULL;
            }
            if(b->data.v_int==1){
                return a;
            }
            int length=strlen(a->data.v_string)*b->data.v_int+1;
            char* newString=calloc(sizeof(char*),length);
            for(int i=0;i<b->data.v_int;i++){
                strcat(newString,a->data.v_string);
            }
            Object_t* obj=new_object_String(newString);
            free(newString);
            return obj;
        }
        case COMPLEX:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Complex(a->data.v_complex.real * b->data.v_int,a->data.v_complex.imag * b->data.v_int);
                case FLOAT:
                    return new_object_Complex(a->data.v_complex.real * b->data.v_float,a->data.v_complex.imag * b->data.v_float);
                case COMPLEX:
                    return new_object_Complex(
                        ((a->data.v_complex.real * b->data.v_complex.real)-(a->data.v_complex.imag * b->data.v_complex.imag)),
                        ((a->data.v_complex.real * b->data.v_complex.imag)+(a->data.v_complex.imag * b->data.v_complex.real))
                    );
                default:
                    return NULL;
            }
        }
        case ARRAY: {
            if(b->kind!=INTEGER){
                return NULL;
            }
            int lenght=a->data.v_array.size*b->data.v_int;
            Object_t* obj=new_object_Array(lenght);
            for(int j=0;j<b->data.v_int;j++){
                for(int i=0;i<a->data.v_array.size;i++){
                    array_set(obj,j+i,array_get(a,i));
                }
            }
            return obj;
        }
        default:
            return NULL;
    }
}

Object_t* divide(Object_t* a,Object_t* b){
    if(a==NULL || b==NULL){
        return NULL;
    }

    switch(a->kind){
        case INTEGER: {
            switch(b->kind){
                case INTEGER:
                    return new_object_Integer(a->data.v_int/b->data.v_int);
                case FLOAT:
                    return new_object_Float((float)a->data.v_int/b->data.v_float);
                default:
                    return NULL;
            }
        }
        case FLOAT:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Float(a->data.v_float/(float)b->data.v_int);
                case FLOAT:
                    return new_object_Float(a->data.v_float/b->data.v_float);
                default:
                    return NULL;
            }
        }
        case COMPLEX:{
            switch(b->kind){
                case INTEGER:
                    return new_object_Complex(a->data.v_complex.real/b->data.v_int,a->data.v_complex.imag/b->data.v_int);
                case FLOAT:
                    return new_object_Complex(a->data.v_complex.real/b->data.v_float,a->data.v_complex.imag/b->data.v_float);
                case COMPLEX:
                    return new_object_Complex(
                        (((a->data.v_complex.real*b->data.v_complex.real)+(a->data.v_complex.imag*b->data.v_complex.imag))/(a->data.v_complex.real*a->data.v_complex.real+a->data.v_complex.imag*a->data.v_complex.imag)),
                        (((a->data.v_complex.real*b->data.v_complex.imag)+(a->data.v_complex.imag*b->data.v_complex.real))/(a->data.v_complex.real*a->data.v_complex.real+a->data.v_complex.imag*a->data.v_complex.imag))
                    );
                default:
                    return NULL;
            }
        }
        case VECTOR:{
            if(b->kind!=VECTOR){
                return NULL;
            }
            return new_object_Vector(
                divide(a->data.v_vector.x,b->data.v_vector.x),
                divide(a->data.v_vector.y,b->data.v_vector.y),
                divide(a->data.v_vector.z,b->data.v_vector.z)               
            );
        }
        default:
            return NULL;
    }
}