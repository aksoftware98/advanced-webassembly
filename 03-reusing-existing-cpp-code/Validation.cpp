#include <cstdlib> 
#include <cstring>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


    // Create the validate value function that will be called to check if the value has a valid value for both product and categroy
    int ValidateValueProvided(const char* value, const char* error_message, char* return_error_message)
    {
        if (value == NULL || value[0] == '\0') {
            strcpy(return_error_message, error_message);
            return 0;
        }

        return 1;
    }

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
    // Create the validate name function that will check if the name validates the maximum length
    int ValidateName(const char* name, const int maximum_length, char* return_error_message) {
        if (ValidateValueProvided(name, "Name is required", return_error_message) == 0) {
            return 0;
        }

        if (strlen(name) > maximum_length) {
            strcpy(return_error_message, "Name is too long");
            return 0;
        }
        return 1;
    }

#ifdef __EMSCRIPTEN__
        EMSCRIPTEN_KEEPALIVE
#endif
    // Create the validate category function that will check if the selected category is valid and existing in the system 
    int ValidateCategory(char* selected_category_id, int* validate_category_ids, int array_length, char* return_error_message) {
        if (ValidateValueProvided(selected_category_id, "Category is required", return_error_message) == 0) {
            return 0;
        }

        if ((validate_category_ids == NULL) || (array_length == 0)) {
            strcpy(return_error_message, "There are no categories available");
            return 0;
        }

        if (IsCategoryInArray(selected_category_id, validate_category_ids, array_length) == 0) {
            strcpy(return_error_message, "Category is not valid");
            return 0;
        }

        return 1;
    }

    // Check if a selected category is existing in array 
    int IsCategoryInArray(char* selected_category_id, int* valid_category_ids, int array_length) {
        int category_id = atoi(selected_category_id);
        for (int index = 0; index < array_length; index++) {
            if (category_id == valid_category_ids[index]) {
                return 1;
            }
        }
        return 0;
    }
#if __cplusplus
}
#endif