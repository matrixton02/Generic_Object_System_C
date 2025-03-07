# Generic Object System with Reference Counting GC  

A lightweight and efficient generic object system in C with a built-in **reference counting garbage collector**.  
This project allows dynamic storage of multiple data types and automatically manages memory through reference counting.  

## ✨ Features  
- **Type-generic storage** using `struct`, `union`, and `enum`.  
- **Reference counting garbage collector** for automatic memory management.  
- **Support for multiple data types** (integers, floats, strings, custom structures).  
- **Efficient memory allocation and deallocation**.  
- **Custom memory allocator (optional)** for better control over heap usage.
- **Fucnctionalities like add substract multiply and divide are overloaded** So the fucntions can be used on different combinations of objects wihtout the user having to manage it.

## 📦 Installation and Running the code
Clone the repository:  
```sh
git clone https://github.com/yourusername/generic-object-system.git
cd generic-object-system
gcc Object_main.c object.c -o program
./program.exe
```
## Fucntions
**Creating a new object** a new object can be created like this Object_t* var_name
**Storing Inetger in our object** an inetger value can be stored using the new_object_Inetger(int value) fucntions here the paramter value is stored in the object.

**Storing Float** a float value can be stored using the new_object_Float(float value) fucntions.

**Storing String** a string value can be stored using the new_object_String(char* value) fucntion.

**Storing Vector** here vector is a custom data type which is a collection of 3 Object_t the fucntion used to create it is
new_object_Vector(Object_t* x,Object_t* y,Object_t* z);

**Storing Complex** complex is also a custom data type which contains two float value representing real and imaginary numbers the creation function is 
new_object_Complex(float a,float b) a represents the real part and b represents the imaginary part.

**Storing an Array** we cannot directly store value in an array first we need to create an array of fixed size and then we can set and get data using 2 more in built fucntions, new_object_Array(int size) here size is the size of the array, the array stores the Object_t* so we can stores Object_t with different data inside togerther in one array.

**Storing data in Array** we can use the array_set(Object_t* obj,int index,Object_t* value) function to store value object at the position index in out array object the function retuns 1 after successfull completion otherwise returns 0.

**Getting data from Array** we can use the array_get(Object_t* obj,int index) to get te obejct from out obj array at position index.

***There are some other in built fucntions like add sub multiply divide that performs operations on different Object***

I have also added some extra fucntionality like adding two string or array object will return theri concatination.
The multiply fucntion when used with one object as string or array and the other as an Inetger then it returns a string or array where the value is repeated the Inetger number of times.

**There is an inbuilt reference counting garbage collection system that keeps tracks of number of references an object has**

**To Know more about the code structure and fucntioanlity you can check out the code**

***Do not make any changes to objec.c and object.h write all your code in Object_main.c and whlile compiling bind the object.c with it using commands as shown in Installation and running the code.***
