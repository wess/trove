/**
 * @file trove.c
 * @brief Implementation of the Trove ARC memory management system
 * 
 * This file contains the implementation of the Automatic Reference Counting (ARC)
 * memory management system defined in trove.h. It provides autorelease pool management
 * and reference counting operations for ARC-managed objects.
 */

#include "trove.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Global pointer to the current active autorelease pool */
AutoreleasePool *current_autorelease_pool = NULL;

/**
 * @brief Autorelease Pool Management
 */

/**
 * @brief Creates and pushes a new autorelease pool onto the stack
 * 
 * This function allocates a new autorelease pool and makes it the current pool.
 * If allocation fails, the program will exit with an error message.
 */
void autorelease_pool_push() {
    AutoreleasePool *pool = (AutoreleasePool *)malloc(sizeof(AutoreleasePool));
    if (!pool) {
        fprintf(stderr, "Failed to allocate autorelease pool.\n");
        exit(1);
    }
    pool->count = 0;
    pool->capacity = 16;  // Initial capacity
    pool->objects = (ARCObject **)malloc(pool->capacity * sizeof(ARCObject *));
    if (!pool->objects) {
        fprintf(stderr, "Failed to allocate autorelease pool objects.\n");
        free(pool);
        exit(1);
    }
    current_autorelease_pool = pool;
}

/**
 * @brief Pops the current autorelease pool, releasing all objects in it
 * 
 * This function releases all objects in the current autorelease pool,
 * frees the pool itself, and sets the current pool to NULL.
 * If there is no current pool, this function does nothing.
 */
void autorelease_pool_pop() {
    if (!current_autorelease_pool)
        return;
    AutoreleasePool *pool = current_autorelease_pool;
    for (size_t i = 0; i < pool->count; i++) {
        arc_release(pool->objects[i]);
    }
    free(pool->objects);
    free(pool);
    current_autorelease_pool = NULL;
}

/**
 * @brief Adds an object to the current autorelease pool
 * 
 * This function adds the given object to the current autorelease pool.
 * If there is no current pool, an error message is printed.
 * The pool will automatically resize if it reaches capacity.
 * 
 * @param obj The object to add to the current autorelease pool
 */
void autorelease_add(ARCObject *obj) {
    if (!current_autorelease_pool) {
        fprintf(stderr, "No autorelease pool in place!\n");
        return;
    }
    AutoreleasePool *pool = current_autorelease_pool;
    if (pool->count >= pool->capacity) {
        pool->capacity *= 2;
        ARCObject **new_objects = (ARCObject **)realloc(pool->objects, pool->capacity * sizeof(ARCObject *));
        if (!new_objects) {
            fprintf(stderr, "Failed to reallocate autorelease pool objects.\n");
            exit(1);
        }
        pool->objects = new_objects;
    }
    pool->objects[pool->count++] = obj;
}

/**
 * @brief ARC Operations
 */

/**
 * @brief Increments the reference count of an object
 * 
 * This function increments the reference count of the given object,
 * indicating that a new reference to the object has been created.
 * If the object is NULL, this function does nothing.
 * 
 * @param obj The object whose reference count should be incremented
 */
void arc_retain(ARCObject *obj) {
    if (obj) {
        obj->ref_count++;
    }
}

/**
 * @brief Decrements the reference count of an object and deallocates if zero
 * 
 * This function decrements the reference count of the given object.
 * If the reference count reaches zero, the object's dealloc function is called.
 * If the object is NULL, this function does nothing.
 * 
 * @param obj The object whose reference count should be decremented
 */
void arc_release(ARCObject *obj) {
    if (obj) {
        obj->ref_count--;
        if (obj->ref_count <= 0) {
            if (obj->dealloc) {
                obj->dealloc(obj);
            }
        }
    }
}

/**
 * @brief Adds an object to the current autorelease pool and returns it
 * 
 * This function adds the given object to the current autorelease pool and
 * returns the object, allowing for convenient chaining in expressions.
 * 
 * @param obj The object to add to the current autorelease pool
 * @return The same object (for convenience in chaining)
 */
ARCObject* arc_autorelease(ARCObject *obj) {
    autorelease_add(obj);
    return obj;
}

/**
 * @brief TroveString Implementation
 */

/**
 * @brief Creates a new ARC-managed string
 * 
 * This function allocates a new TroveString object with a reference count of 1.
 * The string is initialized with the given value, or an empty string if NULL.
 * If allocation fails, the program will exit with an error message.
 * 
 * @param init The initial value for the string (can be NULL)
 * @return A new TroveString with a reference count of 1
 */
TroveString* TroveString_create(const char *init) {
    TroveString *str_obj = (TroveString *)malloc(sizeof(TroveString));
    if (!str_obj) {
        fprintf(stderr, "Failed to allocate TroveString.\n");
        exit(1);
    }
    str_obj->base.ref_count = 1;
    str_obj->base.dealloc = TroveString_dealloc;
    if (init) {
        str_obj->str = strdup(init);
    } else {
        str_obj->str = strdup("");
    }
    if (!str_obj->str) {
        fprintf(stderr, "Failed to allocate string content.\n");
        free(str_obj);
        exit(1);
    }
    return str_obj;
}

/**
 * @brief Deallocates a TroveString
 * 
 * This function frees the memory used by a TroveString object.
 * It first frees the string content, then the TroveString structure itself.
 * This function is called automatically when the reference count reaches zero.
 * 
 * @param obj The object to deallocate (cast to ARCObject)
 */
void TroveString_dealloc(ARCObject *obj) {
    TroveString *str_obj = (TroveString *)obj;
    if (str_obj->str) {
        free(str_obj->str);
    }
    free(str_obj);
}
