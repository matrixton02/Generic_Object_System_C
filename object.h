#ifndef object
#define object
typedef struct Object Object_t;
int object_lenght(Object_t* obj);
typedef enum ObjectKind{
    INTEGER,
    FLOAT,
    STRING,
    VECTOR,
    ARRAY,
    COMPLEX
} Object_Kind_t;

typedef struct ObjectVector{
    Object_t* x;
    Object_t* y;
    Object_t* z;
}Object_vector_t;

typedef struct ObjectArray{
    int size;
    Object_t** elements;
}Object_Array_t;

typedef struct Complex{
    float real;
    float imag;
} Complex_t;

typedef union ObjectData{
    int v_int;
    float v_float;
    char* v_string;
    Object_vector_t v_vector;
    Object_Array_t v_array;
    Complex_t v_complex;
}Object_Data_t;

typedef struct Object{
    int refcount;
    Object_Kind_t kind;
    Object_Data_t data;
}Object_t;

Object_t* new_object_Integer(int value);
Object_t* new_object_Float(float value);
Object_t* new_object_String(char* value);
Object_t* new_object_Vector(
    Object_t* x,
    Object_t* y,
    Object_t* z
);
Object_t* new_object_Complex(float a,float b);
Object_t* new_object_Array(int size);
int array_set(Object_t* obj,int index,Object_t* value);
Object_t* array_get(Object_t* obj,int index);
Object_t* add(Object_t* a,Object_t* b);
Object_t* sub(Object_t* a,Object_t* b);
Object_t* multiply(Object_t* a,Object_t* b);
void refcount_inc(Object_t* obj);
void refcount_dec(Object_t* obj);
void object_free(Object_t* obj);
#endif