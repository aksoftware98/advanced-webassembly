# Executing JavaScript logic from C++
To be able to call a JS method from the C++ code, there is 3 possible ways:
- **Emscripten** macros (*EM_JS* & *EM_ASM*)
- Add custom JS to the JS Emscripten file to call directly
- Use functions pointer


## Use Emscripten Micros 
To be able to call a function in C++ that has to be imported from JavaScript, we need to defnie its signature while using the **extern** keyword ahead of it, in this case the C++ code will excpect the function to be available at the runtime and the Emscripten will make sure to mark as to be imported while instantiating the JS

