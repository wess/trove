/**
 * @file main.c
 * @brief Example usage of the Trove ARC memory management system
 * 
 * This file demonstrates how to use the Trove Automatic Reference Counting (ARC)
 * system in a simple program. It shows how to create ARC-managed objects within
 * an autorelease pool block using the TROVE macro.
 */

#include "macros.h"
#include <stdio.h>

/**
 * @brief Program entry point
 * 
 * Demonstrates the use of the TROVE macro to create an autorelease pool block,
 * and the String macro to create an autoreleased TroveString object.
 * 
 * @return Exit status code
 */
int main() {
    TROVE {
        // Within this block, an autorelease pool is active.
        // Any autoreleased objects created within the block will be
        // automatically released when the block ends.
        TroveString *greeting = String("Hello, trove ARC with TROVE macro!");
        printf("%s\n", greeting->str);
        // No need to manually release greeting - it will be released
        // automatically when the TROVE block ends
    }
    return 0;
}
