/**
 * @file macros.h
 * @brief Convenience macros for the Trove ARC memory management system
 * 
 * This file defines a set of macros that simplify working with the Trove ARC
 * (Automatic Reference Counting) memory management system. These macros provide
 * convenient syntax for creating objects, managing memory, and working with
 * autorelease pools.
 */

#ifndef MACROS_H
#define MACROS_H

#include "trove.h"

/**
 * @brief Creates an autoreleased TroveString
 * 
 * This macro creates a new TroveString object with the given text
 * and adds it to the current autorelease pool.
 * 
 * @param text The initial text for the string
 * @return An autoreleased TroveString object
 * 
 * @code
 * TroveString *s = String("Hello, ARC!");
 * @endcode
 */
#define String(text) ((TroveString *)arc_autorelease((ARCObject *)TroveString_create(text)))

/**
 * @brief Generic macro to create any ARC-managed object
 * 
 * This macro creates a new object of the specified type using the type's create
 * function and adds it to the current autorelease pool. It assumes that the type
 * has a create function named type_create(...).
 * 
 * @param type The type of object to create
 * @param ... Arguments to pass to the type's create function
 * @return An autoreleased object of the specified type
 * 
 * @code
 * TroveArray *array = ARC_NEW(TroveArray, 10);
 * @endcode
 */
#define ARC_NEW(type, ...) ((type *)arc_autorelease((ARCObject *)type##_create(__VA_ARGS__)))

/**
 * @brief Convenience macros for ARC memory management
 */

/**
 * @brief Increments the reference count of an object
 * 
 * @param obj The object whose reference count should be incremented
 */
#define RETAIN(obj)  arc_retain((ARCObject *)(obj))

/**
 * @brief Decrements the reference count of an object
 * 
 * @param obj The object whose reference count should be decremented
 */
#define RELEASE(obj) arc_release((ARCObject *)(obj))

/**
 * @brief Convenience macros for autorelease pool management
 */

/**
 * @brief Creates a new autorelease pool and makes it current
 */
#define AUTORELEASE_POOL_PUSH() autorelease_pool_push()

/**
 * @brief Releases all objects in the current autorelease pool and removes it
 */
#define AUTORELEASE_POOL_POP()  autorelease_pool_pop()

/**
 * @brief Creates a scoped autorelease pool block
 * 
 * This macro uses a for-loop trick to create a block with an autorelease pool
 * that is automatically pushed at the beginning of the block and popped at the
 * end of the block. The code inside the block is executed exactly once.
 * 
 * @code
 * TROVE {
 *     // Code within this block uses a temporary autorelease pool
 *     // All autoreleased objects created here will be released
 *     // when execution leaves this block
 *     TroveString *str = String("Hello, world!");
 *     // No need to manually release str
 * }
 * @endcode
 */
#define TROVE for (int _trove_once = (autorelease_pool_push(), 1); _trove_once; autorelease_pool_pop(), _trove_once = 0)

#endif // MACROS_H
