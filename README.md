# Trove ARC

A lightweight Automatic Reference Counting (ARC) memory management system for C, inspired by Objective-C/Swift ARC patterns. Trove provides reference counting and autorelease pool functionality for C objects, simplifying memory management.

## Features

- **Automatic Reference Counting**: Track object lifetimes with retain/release semantics
- **Autorelease Pools**: Defer object deallocation for convenient memory management
- **Scoped Memory Management**: TROVE macro creates scoped autorelease blocks
- **Type-Safe API**: Consistent interface for creating and managing object lifecycles
- **Minimal Dependencies**: Standard C library only with no external dependencies

## Building and Installation

Clone the repository and build using make:

```bash
git clone https://github.com/yourusername/trove.git
cd trove
make
```

The project will build a sample program in the `build` directory.

## Usage

### Basic Example

```c
#include "macros.h"
#include <stdio.h>

int main() {
    TROVE {
        // Create an autoreleased string
        TroveString *greeting = String("Hello, Trove ARC!");
        printf("%s\n", greeting->str);
        
        // No need to free memory - automatically handled when TROVE block ends
    }
    return 0;
}
```

### Memory Management

```c
// Create an object with reference count 1
TroveString *str = TroveString_create("Manually managed");

// Increase reference count
RETAIN(str);

// Decrease reference count
RELEASE(str);

// Release again (reference count now 0, object is freed)
RELEASE(str);
```

### Using Autorelease Pools

```c
// Push a new autorelease pool
AUTORELEASE_POOL_PUSH();

// Create autoreleased objects
TroveString *str1 = String("Object 1");
TroveString *str2 = String("Object 2");

// Use objects...

// Pop the autorelease pool, releasing all autoreleased objects
AUTORELEASE_POOL_POP();
```

## Core API

### Objects

- `ARCObject`: Base structure for all ARC-managed objects
- `TroveString`: Example implementation of an ARC-managed string type

### Memory Operations

- `arc_retain()`: Increment an object's reference count
- `arc_release()`: Decrement an object's reference count and free if zero
- `arc_autorelease()`: Add an object to the current autorelease pool

### Convenience Macros

- `RETAIN(obj)`: Retain an object
- `RELEASE(obj)`: Release an object
- `String(text)`: Create an autoreleased string
- `TROVE { ... }`: Create a scoped autorelease pool block
- `AUTORELEASE_POOL_PUSH()`: Push a new autorelease pool
- `AUTORELEASE_POOL_POP()`: Pop the current autorelease pool

## Extending Trove

To create your own ARC-managed objects:

1. Include `ARCObject` as the first member of your struct
2. Implement a create function that sets up the object with a reference count of 1
3. Implement a dealloc function that frees resources when reference count reaches zero
4. Optionally create convenience macros using `arc_autorelease()`

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
