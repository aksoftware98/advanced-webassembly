# Use function pointers
By creating a function pointers in the C++ code (Delegates in C#)
```cplusplus
typedef void (*OnError) (void);
typedef int (*ConverToInt) (const char* buffer);
```

In this case we can pass those pointers to other methods like 
```cplusplus
void SaveChanges(const int value, OnError OnErrorCallback) {
    // Logic goes here
}
```

In the plubmping file, Emscripten stores the function pointers within an array and we need to define how many pointers we need at the same time in the compile time, and to do that we have to use the following argument in the comiplation
```ps
emcc Validation.cpp -s RESERVED_FUNCTION_POINTERS=4 -s EXPORTED_RUNTIME_METHODS="['ccall', 'UTF8ToString', 'addFunction', 'removeFunction']" -s EXPORTED_FUNCTIONS="['_malloc', '_free']" -o validate.js
```

The functions ***addFunction*** and the ***removeFunction*** allows to add a function to the backing array of the plumping file, the ***addFunction*** returns the index of the pointer in the array, so we need to store it so we can call the ***removeFunction***

