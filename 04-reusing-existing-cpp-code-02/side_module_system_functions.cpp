#include <stdio.h>
#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif 

const int TOTAL_MEMORY = 65536;
const int MAXIMUM_ALLOCATED_CHUNKS = 10;

int current_allocated_count = 0;

struct MemoryAllocated {
    int offset;
    int size;
};

// Declare the array of the current allocated memory chunks
struct MemoryAllocated AllocatedMemoryChunks[MAXIMUM_ALLOCATED_CHUNKS];

void InsertIntoAllocatedArray(int new_item_index, int offset, int size_needed) {
    for(int i = (MAXIMUM_ALLOCATED_CHUNKS - 1); i > new_item_index; i--) {
        AllocatedMemoryChunks[i] = AllocatedMemoryChunks[i - 1];
    }
    AllocatedMemoryChunks[new_item_index].offset = offset;
    AllocatedMemoryChunks[new_item_index].size = size_needed;

    current_allocated_count++;
}

/// @brief Try to find if there is enough memory to allocate the requested size, if so then return the offset
/// @param size_needed 
/// @return 
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
int create_buffer(int size_needed) {
    if (current_allocated_count == MAXIMUM_ALLOCATED_CHUNKS) {
        return 0;
    }

    int offset_start = 1024; // Because the first 1024 bytes are reserved for the WebAssembly literal strings when Emscripten is used
    int current_offset = 0;
    int found_room = 0; 

    int memory_size = size_needed;
    // We need to increase the size of the memory so it's a multiple of 8 to fit the remaining data we need to store
    while (memory_size % 8 != 0) { memory_size++; }

    // Check if there is a room in the allocated memory chunks 
    for (int index = 0; index < current_allocated_count; index++) {
        current_offset = AllocatedMemoryChunks[index].offset;
        if ((current_offset - offset_start) >= 0) {
            InsertIntoAllocatedArray(index, offset_start, memory_size);
            found_room = 1;
            break;
        }

        offset_start = current_offset + AllocatedMemoryChunks[index].size;
    }

    // If no room found between the allocated memory chunks then add a new memroy chunk
    if (found_room == 0) {
        if (((TOTAL_MEMORY - 1) - offset_start) >= size_needed) {
            AllocatedMemoryChunks[current_allocated_count].offset = offset_start;
            AllocatedMemoryChunks[current_allocated_count].size = size_needed;
            found_room = 1;
        }
    }

    if (found_room == 1) {
        return offset_start;
    }

    return 0;
}

#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_KEEPALIVE
#endif
void free_buffer(int offset) {
    int shift_left = 0;

    for (int index = 0; index < current_allocated_count; index++) {
        if (AllocatedMemoryChunks[index].offset == offset) {
            shift_left = 1;
        }

        if (shift_left == 1) {
            if (index < current_allocated_count - 1) {
                AllocatedMemoryChunks[index] = AllocatedMemoryChunks[index + 1];
            }
            else {
                AllocatedMemoryChunks[index].offset = 0;
                AllocatedMemoryChunks[index].size = 0;
            }
        }
    }

    current_allocated_count--;
}

/// @brief Copy a C++ string array to another one 
/// @param destination 
/// @param source 
/// @return 
char* strcpy(char* destination, const char* source) {
    char* return_copy = destination;
    while (*source) {*destination++ = *source++; }
    *destination = 0;

    return return_copy;
}

/// @brief Return the length of a given string
/// @param value 
/// @return 
size_t strlen(const char* value) {
    size_t count = 0;
    while(value[count] != '\0') { count++; }
    return count;
}

int atoi(const char* value) {
    if ((value == NULL) || value[0] == '\0') {
        return 0;
    }
    int result = 0;
    int sign = 0;

    // check if the first character is the - sign 
    if (*value == '-') {
        sign = -1;
        ++value;
    }

    char current_value = *value; 
    while(current_value != '\0') {
        if ((current_value >= '0') && current_value <= '9') {
            result = result * 10 + current_value - '0';
            ++value;
            current_value = *value;
        }
        else {
            return 0;
        }
    }

    if (sign == -1) {
         result = result * -1;
    }

    return result;
}


// 10 - 1 i => 9, new = 0; 
#ifdef __cplusplus 
}
#endif