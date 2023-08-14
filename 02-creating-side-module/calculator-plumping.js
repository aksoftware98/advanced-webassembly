

// This code uses the instantiatStreaming that directly downloads the file and compile it on the go
// In some old browsers instatiateStreaming maybe not available so we need to use instatiate

// const importObject = {
//     env: {
//         memoryBase: 0,
//     }
// };
// WebAssembly.instantiateStreaming(fetch("side-module.wasm"), importObject)
//     .then(result => {
//         const value = result.instance.exports._Increment(17);
//         console.log(value.toString());
//     });

function isWebAssemblySupported() {
    try {
        if (typeof WebAssembly === "object") {
            const module = new WebAssembly.Module(Uint8Array.of(0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00));
            if (module instanceof WebAssembly.Module) {
                return new WebAssembly.Instance(module) instanceof WebAssembly.Instance
            }
        }
    } catch (err) {}

    return false;
}