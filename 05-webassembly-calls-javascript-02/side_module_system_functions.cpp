#include <stdio.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif
#ifdef __cplusplus
extern "C" { // So that the C++ compiler does not rename our function names
#endif

  const int TOTAL_MEMORY = 65536; // We should always have at least 1 page of memory (1,024 bytes x 64 KiB).
  const int MAXIMUM_ALLOCATED_CHUNKS = 10;
  static int current_allocated_count = 0; // We have to mark as static as the Emscriptten is thinking this is global and to be imported in other moduels

  struct MemoryAllocated 
  {
    int offset;
    int length;
  };

  static struct MemoryAllocated AllocatedMemoryChunks[MAXIMUM_ALLOCATED_CHUNKS];// The array that will hold details about each memory allocation

  // Helper when adding an item before existing items in the array
  void InsertIntoAllocatedArray(int new_item_index, int offset_start, int size_needed)
  {
    // Shift everything to the right by one if it is to the right of where the new item will be inserted
    for (int i = (MAXIMUM_ALLOCATED_CHUNKS - 1); i > new_item_index; i--)
    {
      AllocatedMemoryChunks[i] = AllocatedMemoryChunks[(i - 1)];
    }

    // Add the new item at the specified index
    AllocatedMemoryChunks[new_item_index].offset = offset_start;
    AllocatedMemoryChunks[new_item_index].length = size_needed;

    // Increment the count of blocks allocated
    current_allocated_count++;
  }

  // Our version of malloc
  #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    #endif
  int create_buffer(int size_needed)
  {
    // If we are already at our limit of allocated memory blocks then exit now
    if (current_allocated_count == MAXIMUM_ALLOCATED_CHUNKS) { return 0; }

    // Adjust the start position to give room for items that will be copied into memory when the module is instantiated
    int offset_start = 1024;
    int current_offset = 0;
    int found_room = 0;

    int memory_size = size_needed;
    while (memory_size % 8 != 0) { memory_size++; }// Increase the size so that the next offset will be a multiple of 8

    // Loop through the currently allocated memory...
    for (int index = 0; index < current_allocated_count; index++)
    {
      // If there is space between the previous offset and the current one for the memory that is wanted then...
      current_offset = AllocatedMemoryChunks[index].offset;
      if ((current_offset - offset_start) >= memory_size)
      {
        // Add the current item to the current index of the array (bump the rest of the items to the right by one)
        InsertIntoAllocatedArray(index, offset_start, memory_size);
        found_room = 1;
        break;
      }

      // OffsetStart for the next loop will be the end of the current array item's memory block
      offset_start = (current_offset + AllocatedMemoryChunks[index].length);
    }

    // Room wasn't found in between the existing allocated memory blocks
    if (found_room == 0)
    {
      // If there is room between the end of the last memory block and the end of memory then...
      if (((TOTAL_MEMORY - 1) - offset_start) >= size_needed)
      {
        // Add the item to the array
        AllocatedMemoryChunks[current_allocated_count].offset = offset_start;
        AllocatedMemoryChunks[current_allocated_count].length = size_needed;
        current_allocated_count++;
        found_room = 1;
      }
    }

    // If there was room for the memory needed then return the offset
    if (found_room == 1) { return offset_start; }

    // Otherwise, tell the caller there was no room (NULL pointer)
    return 0;
  }

  // Our version of free (for our version of malloc)
  #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    #endif
  void free_buffer(int offset)
  {
    int shift_item_left = 0; // Don't shift left by default

    // Loop through the currently allocated memory...
    for (int index = 0; index < current_allocated_count; index++)
    {
      // If the current item's offset matches the one we wish to free then we want to start shifting all items to the left starting here
      if (AllocatedMemoryChunks[index].offset == offset) { shift_item_left = 1; }

      // If we are to shift from the left then...
      if (shift_item_left == 1)
      {
        // If there is at least one more item in the array to the right then...
        if (index < (current_allocated_count - 1))
        {
          AllocatedMemoryChunks[index] = AllocatedMemoryChunks[(index + 1)];
        }
        else // We're at the end of the array. Zero out the values.
        { 
          AllocatedMemoryChunks[index].offset = 0;
          AllocatedMemoryChunks[index].length = 0;
        }
      }
    }

    // If we shifted left that means we found the offset requested and removed it from the array. Adjust the count of items in the array
    current_allocated_count--;
  }

  // Our version of strcpy
  char* strcpy(char* destination, const char* source)
  {
    // Loop until we have copied all of the source characters to the destination
    char* return_copy = destination;
    while (*source) { *destination++ = *source++; }
    *destination = 0;// Null terminate the string since we only looped until we hit the null terminator in the source (we didn't copy the null terminator)

    return return_copy;
  }

  // Our version of strlen
  size_t strlen(const char* value)
  {
    // Loop while the current character is not NULL
    size_t length = 0;
    while (value[length] != '\0') { length++; }

    // Return the count
    return length;
  }

  // our version of atoi
  #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    #endif
  int atoi(const char* buffer)
  {
    if (buffer == NULL || buffer[0] == '\0') {
     return 0;
    }

    int value = 0;
    char current_char = *buffer;
    int is_negative = 0;

    // Check if it's negative 
    if (current_char == '-') {
        is_negative = 1;
        buffer++;
    }

    while (current_char != '\0') {
        value = value * 10 + current_char - '0';
        buffer++;
        current_char = *buffer;
    }

    if (is_negative == 1) {
        value = -value;
    }

    return value;
  }

  #ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    #endif
  char* int_to_string(int value, char* buffer, int buffer_size) {
    int current = 0;
int index = 0;
int is_negative = 0;

// Check if the value is negative 
if (value < 0) {
    is_negative = 1;
    value = -value;
}

// Add each char to the values 
do {
    buffer[index++] = value % 10 + '0';
    value /= 10;
} while (value > 0 && index < buffer_size - 1);

// Add the negative sign if it's negative 
if (index < buffer_size - 1 && is_negative == 1) {
    buffer[index++] = '-';
}

// Reverse the buffer array 
int length = index;
for (int j = 0; j < length / 2; j++) {
    int temp = buffer[j];
    buffer[j] = buffer[length - j - 1];
    buffer[length - j - 1] = temp;
}
buffer[index] = '\0';
return buffer;
  }

#ifdef __cplusplus
}
#endif
