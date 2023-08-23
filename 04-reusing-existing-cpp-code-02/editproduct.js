// Simulate retrieving data from a database
const initialData = {
    name: "Diadora Men Jeans",
    categoryId: "1"
}

let moduleMemory = null;
let moduleExports = null; 

const MAXIMIM_NAME_LENGTH = 50;
const VALIDE_CATEGORY_IDS = [1, 2, 3125];

function isWebAssemblySupported() {
    try {
    if (typeof WebAssembly === "object") {
        var module = new WebAssembly.Module(Uint8Array.of(0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00));
        if (module instanceof WebAssembly.Module)
            return new WebAssembly.Instance(module) instanceof WebAssembly.Instance;
    }
    }
    catch (err) {

    }
}



function initializePage() {
    var isSupported = isWebAssemblySupported();
    console.log("WebAssembly supported: " + isSupported);
    // Setup WebAssembly 
    moduleMemory  = new WebAssembly.Memory({initial: 256, maximum: 256});

    const importObject = {
        env: {
            __memory_base: 0,
            memory: moduleMemory,
            __stack_pointer: new WebAssembly.Global({value: 'i32', mutable: true}, 0)
        },
    }
    if (!isSupported) {
        console.log("WebAssembly is not supported");
        return;
    }

    WebAssembly.instantiateStreaming(fetch('validate.wasm'), importObject)
        .then(obj => {
            moduleExports = obj.instance.exports;
        });

    document.getElementById("name").value = initialData.name;
    const category = document.getElementById("category");
    const count = category.length;

    for (let i = 0; i < count; i++) {
        if (category[i].value === initialData.categoryId) {
            category.selectedIndex = i;
            break;
        }
    }
    
}

function getSelectedCategoryId() {
    const category = document.getElementById("category");
    const index = category.selectedIndex;
    if (index !== -1)
        return category[index].value;
    return "0"
}

function setErrorMessage(error) {
    const errorElement = document.getElementById("errorMessage");
    errorElement.innerHTML = error;
    errorElement.style.display = (error === "" ? "none" : "block");
}

function onClickSave() {
    const name = document.getElementById("name").value;
    const categoryId = getSelectedCategoryId();
   
    let errorMessage = "";
    const errorMessagePointer = moduleExports.create_buffer(256);
   
    if (!validateName(name, errorMessagePointer) || !validateCategory(categoryId, errorMessagePointer)) {
        errorMessage = getStringFromMemory(errorMessagePointer);
    }
    moduleExports.free_buffer(errorMessagePointer);
    setErrorMessage(errorMessage);

    if (errorMessage === "") {
        // Save the data to the server
        alert("Save successfully!");
    }
}

/// Read a string from the module of the memory
function getStringFromMemory(offset) {
    const size = 256; 
    const bytes = new Uint8Array(moduleMemory.buffer, offset, size);
    let returnValue = "";
    let character = "";
    for (let i = 0; i < size; i++) {
        character = String.fromCharCode(bytes[i]);
        if (character === "\0") {
            break;
        }
        returnValue += character;
    }

    return returnValue;
}

// Copy string to memory 
function copyStringToMemory(value, offset) {
    const bytes = new Uint8Array(moduleMemory.buffer);
    bytes.set(new TextEncoder().encode((value + "\0")), offset);
}

function validateName(name, errorMessagePointer) {
    const namePointer = moduleExports.create_buffer(name.length + 1);
    copyStringToMemory(name, namePointer);
    // const isValid = Module.ccall('ValidateName', 'number', ['string', 'number', 'number'], [name, MAXIMIM_NAME_LENGTH, errorMessagePointer]);
    const isValid = moduleExports.ValidateName(namePointer, MAXIMIM_NAME_LENGTH, errorMessagePointer);
    moduleExports.free_buffer(namePointer);
    return isValid === 1;
}

function validateCategory(categoryId, errorMessagePointer) {
  const categoryIdPointer = moduleExports.create_buffer((categoryId.length + 1));
  copyStringToMemory(categoryId, categoryIdPointer);

  const arrayLength = VALIDE_CATEGORY_IDS.length;
  const bytesPerElement = Int32Array.BYTES_PER_ELEMENT;
  const arrayPointer = moduleExports.create_buffer((arrayLength * bytesPerElement));
  
  const bytesForArray = new Int32Array(moduleMemory.buffer);
  bytesForArray.set(VALIDE_CATEGORY_IDS, (arrayPointer / bytesPerElement));
  // Read an int value from the bytesForArray at the offset 4 
   const value = bytesForArray.at(arrayPointer / bytesPerElement + 2);
   console.log(value);
  const isValid = moduleExports.ValidateCategory(categoryIdPointer, arrayPointer, arrayLength, errorMessagePointer);

  moduleExports.free_buffer(arrayPointer);
  moduleExports.free_buffer(categoryIdPointer);

  return (isValid === 1);
}