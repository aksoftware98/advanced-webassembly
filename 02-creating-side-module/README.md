# Generate only a side-module WASM
This excerise will generate only a side-module. The Emsicrpten toolkit will generate only the .wasm file without the plumping file or the HTML. 
This is great excerise for linking two modules or exprorting functions to be used by other WASM modules 

Following is the command to compile the C++ file as a side-module, and it will make the Increment function to be exported so it can be used by JavaScript.
-O1 means the optimizing level of the generated WASM file

```PS
emcc calculator.cpp -s SIDE_MODULE=2 -O1 -s EXPORTER_FUNCTIONS=['_Increment'] -o side-module.wasm
```