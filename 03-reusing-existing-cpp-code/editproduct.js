// Simulate retrieving data from a database
const initialData = {
    name: "Diadora Men Jeans",
    categoryId: "1"
}

const MAXIMIM_NAME_LENGTH = 50;
const VALIDE_CATEGORY_IDS = [1, 2];

function initializePage() {
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
    const errorMessagePointer = _malloc(256);
    alert(errorMessagePointer);
    if (!validateName(name, errorMessagePointer) || !validateCategory(categoryId, errorMessagePointer)) {
        errorMessage = Module.UTF8ToString(errorMessagePointer);
    }
    _free(errorMessagePointer);
    setErrorMessage(errorMessage);

    if (errorMessage === "") {
        // Save the data to the server
        alert("Save successfully!");
    }
}

function validateName(name, errorMessagePointer) {
    const isValid = Module.ccall('ValidateName', 'number', ['string', 'number', 'number'], [name, MAXIMIM_NAME_LENGTH, errorMessagePointer]);
    return isValid === 1;
}

function validateCategory(selectedCategoryid, errorMessagePointer) {
    const arrayLength = VALIDE_CATEGORY_IDS.length; 
    const bytesPerElement = Module.HEAP32.BYTES_PER_ELEMENT;
    const arrayPointer = Module._malloc((bytesPerElement * arrayLength));
    Module.HEAP32.set(VALIDE_CATEGORY_IDS, (arrayPointer / bytesPerElement));

    const isValid = Module.ccall('ValidateCategory', 'number', ['string', 'number', 'number', 'number'], [selectedCategoryid, arrayPointer, arrayLength, errorMessagePointer]);
    _free(arrayPointer);
    return isValid === 1;
}