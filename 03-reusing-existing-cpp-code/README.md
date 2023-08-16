# Reusing an existing C++ code 
After creating the C++ file with all the needed functions two things to mention here that are important 

Use the following directive so we don't have to tell the Emscripten compiler which methods we want to export so we can call from JS
The methods will be included automatically 

```cplusplus
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE 
#endif
```

The other thing to mention is the helper library of EMSCRIPTEN 
```cplusplus
#ifdef __EMSCRIPTEN__
    #inlcude <emscripten.h>

#endif
```

The C++ method overloading feature will lead to having different name for the exproted method, so we might want to instruct the compiler to keep the name as it's using the extern "C" 
```
#ifdef cplusplus
    extern "C" { 
        // The method goes here
    }
#endif
```
