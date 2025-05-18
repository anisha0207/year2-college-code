#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    /* 
     * This program demonstrates how command-line arguments are stored in memory:
     * 1. The argv parameter itself is a pointer to an array of strings
     * 2. Each argv[i] is a pointer to a null-terminated string
     * 3. The array ends with a NULL pointer (argv[argc] == NULL)
     */

    // Print address of argv itself
    // %p format specifier for pointers and cast to void*
    printf("argv: %p\n", (void*)&argv);  // Address of the argv pointer variable
    
    /*
     * Print addresses of argv[i] and their values
     * loop until argc to show the terminating NULL pointer
     */
    for (int i = 0; i <= argc; i++) {
        if (argv[i] == NULL) {
            // The NULL terminator after all arguments
            printf("argv[%d]: %p (NULL)\n", i, (void*)argv[i]);
        } else {
            // Each argument string pointer and its content
            printf("argv[%d]: %p (%s)\n", i, (void*)argv[i], argv[i]);
        }
    }
    
    /*
     * Print addresses of individual characters in each argument string
     * strings are laid out in memory (contiguous bytes)
     * null terminator at the end of each string
     */
    for (int i = 0; i < argc; i++) {
        for (int j = 0; ; j++) {
            // Special formatting for non-printable characters
            char display_char = argv[i][j];
            const char* format = (display_char == '\0') 
                ? "argv[%d][%d]: %p ('\\0')\n"  // Show null terminator explicitly
                : "argv[%d][%d]: %p ('%c')\n";
            
            printf(format, i, j, (void*)&argv[i][j], display_char);
            
            if (argv[i][j] == '\0') break;  // Stop at string terminator
        }
    }
    
    /*
     * argv and its strings are in stack/static memory
     * Heap allocations come from different memory
     */
    char* heap_str = malloc(10);  // Allocate 10 bytes on the heap
    if (heap_str == NULL) {
        perror("malloc failed");
        return 1;
    }
    
    printf("\nHeap allocated address: %p\n", (void*)heap_str);
    
    // Calculate the difference between heap and argv[0] addresses
    // Note: We cast to long for arithmetic, but pointer differences should
    // technically use ptrdiff_t. This is simplified for demonstration.
    printf("Difference from argv[0]: %ld bytes\n", 
           (long)heap_str - (long)argv[0]);
    
    free(heap_str);  // Always free allocated memory
    return 0;
}