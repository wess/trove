/**
 * @file trove.h
 * @brief Core implementation of Trove ARC (Automatic Reference Counting) memory management system.
 * 
 * This header defines the fundamental structures and functions for Trove's Automatic Reference 
 * Counting (ARC) memory management system. It provides a reference-counting mechanism for C objects
 * with autorelease pool functionality similar to Objective-C/Swift ARC.
 */

#ifndef TROVE_H
#define TROVE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Base object for all ARC-managed objects
 * 
 * This structure serves as the base for all objects that will be managed by the
 * ARC system. It contains a reference count and a deallocation function pointer.
 * All ARC-managed objects must have this structure as their first member.
 */
typedef struct ARCObject {
    int ref_count;                        /**< Current reference count */
    void (*dealloc)(struct ARCObject *obj); /**< Function called when refcount reaches zero */
} ARCObject;

/**
 * @brief Autorelease pool structure
 * 
 * Represents an autorelease pool that temporarily holds references to objects,
 * releasing them when the pool is popped. This enables deferred cleanup of objects.
 */
typedef struct AutoreleasePool {
    ARCObject **objects;  /**< Array of autoreleased objects */
    size_t count;         /**< Current number of objects in the pool */
    size_t capacity;      /**< Current capacity of the objects array */
} AutoreleasePool;

/** @brief Pointer to the current active autorelease pool */
extern AutoreleasePool *current_autorelease_pool;

/**
 * @brief Creates and pushes a new autorelease pool onto the stack
 * 
 * This function allocates a new autorelease pool and makes it the current pool.
 * Objects that are autoreleased will be added to this pool.
 */
void autorelease_pool_push();

/**
 * @brief Pops the current autorelease pool, releasing all objects in it
 * 
 * This function releases all objects in the current autorelease pool and
 * frees the pool itself.
 */
void autorelease_pool_pop();

/**
 * @brief Adds an object to the current autorelease pool
 * 
 * @param obj The object to add to the current autorelease pool
 */
void autorelease_add(ARCObject *obj);

/**
 * @brief ARC Operations
 * 
 * Core functions for Automatic Reference Counting memory management.
 */

/**
 * @brief Increments the reference count of an object
 * 
 * @param obj The object whose reference count should be incremented
 */
void arc_retain(ARCObject *obj);

/**
 * @brief Decrements the reference count of an object and deallocates if zero
 * 
 * @param obj The object whose reference count should be decremented
 */
void arc_release(ARCObject *obj);

/**
 * @brief Adds an object to the current autorelease pool
 * 
 * This function adds the given object to the current autorelease pool,
 * ensuring it will be released when the pool is popped.
 * 
 * @param obj The object to autorelease
 * @return The same object (for convenience in chaining)
 */
ARCObject* arc_autorelease(ARCObject *obj);

/**
 * @brief String type managed by ARC
 * 
 * A simple string type that demonstrates how to implement an ARC-managed object.
 * The ARCObject base must be the first member to allow for type casting between
 * ARCObject and derived types.
 */
typedef struct TroveString {
    ARCObject base;   /**< Inheritance: must be the first member */
    char *str;        /**< Null-terminated C string */
} TroveString;

/**
 * @brief Creates a new ARC-managed string
 * 
 * @param init The initial value for the string (can be NULL)
 * @return A new TroveString with a reference count of 1
 */
TroveString* TroveString_create(const char *init);

/**
 * @brief Deallocates a TroveString
 * 
 * This function is called automatically when the reference count reaches zero.
 * 
 * @param obj The object to deallocate (cast to ARCObject)
 */
void TroveString_dealloc(ARCObject *obj);

/**
 * @brief Convenience macro for creating autoreleased TroveString objects
 * 
 * This macro creates a new TroveString and adds it to the current autorelease pool.
 * 
 * @param text The initial value for the string
 * @return An autoreleased TroveString
 * 
 * @code
 * TroveString *s = String("Hello, world!");
 * @endcode
 */
#define String(text) ((TroveString *)arc_autorelease((ARCObject *)TroveString_create(text)))

#endif // TROVE_H
