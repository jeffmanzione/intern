# Intern: A Macro-Based Interning Library for C

**Intern** is a lightweight C library for creating *interning data structures* — collections that store unique instances of data (such as strings or other value types) efficiently.  
It uses preprocessor macros to generate type-safe intern tables backed by hash sets and chunk-based memory allocation.

---

## What Is Interning?

Interning is a technique that ensures identical data values (like strings or structs) are stored only once in memory.  
Instead of duplicating copies, you “intern” the value and reuse the existing one, enabling fast equality checks (pointer comparison) and reducing memory usage.

---

## Features

- **Type-safe macros** — Define interners for any data type.
- **Memory-chunk allocator** — Efficiently allocates large memory blocks for interned data.
- **Custom hash & compare functions** — Plug in your own functions for different data types.
- **Lightweight dependency** — Uses only standard C libraries.
- **Single-header convenience** — Just include `intern.h` and define your intern type.

---

## Example Usage

```c
#include "intern/intern.h"
#include <stdio.h>
#include <string.h>

// Define an Intern for strings (char arrays)
DEFINE_INTERN(StringIntern, char)
IMPL_INTERN(StringIntern, char)

// Simple string hash and compare
uint32_t str_hash(const char *s, uint32_t size) {
    uint32_t hash = 5381;
    for (uint32_t i = 0; i < size; ++i)
        hash = ((hash << 5) + hash) + s[i];
    return hash;
}

int str_compare(const char *a, uint32_t a_size, const char *b, uint32_t b_size) {
    return memcmp(a, b, max(a_size, b_size));
}

int main(void) {
    StringIntern intern;
    StringIntern_init(&intern, str_hash, str_compare);

    const char *a = "hello";
    const char *b = "hello";

    const char *interned_a = StringIntern_intern(&intern, a, strlen(a) + 1);
    const char *interned_b = StringIntern_intern(&intern, b, strlen(b) + 1);

    printf("Pointers equal? %s\n", (interned_a == interned_b) ? "yes" : "no");

    StringIntern_finalize(&intern);
    return 0;
}
````

### Output:

```
Pointers equal? yes
```

---

## 🧩 API Overview

Each intern is generated using macros:

| Macro                             | Purpose                                                             |
| --------------------------------- | ------------------------------------------------------------------- |
| `DEFINE_INTERN(name, value_type)` | Declares a new intern structure and API for the given type.         |
| `IMPL_INTERN(name, value_type)`   | Implements the intern structure. Should be placed in one `.c` file. |

Each generated intern provides:

```c
void name_init(name *intern, name##HashFn hash, name##CompareFn compare);
void name_finalize(name *intern);
value_type *name_intern(name *intern, const value_type *value, uint32_t value_size);
```

---

## Implementation Details

Interns use:

* A **hash set** (`name##HashSet`) to ensure uniqueness.
* A **linked list of data chunks** (`name##Chunk`) for efficient memory allocation.
* A **simple pointer bump allocator** within each chunk.

---

## Integration

1. Include the project in your MODULE.bazel file.
   
   ```
   bazel_dep(name = "intern", version = "0.0.1")
   ```
2. Add the dependency to your cc_library:

   ```
   cc_library(
      name = "my_lib",
      hdrs = [...],
      srcs = [...],
      deps = [
        ...
        "@intern//intern",
      ],
   )
   ```
3. Include the header in your code:

   ```
   #include "intern/intern.h"
   ```
4. Link with your existing C project (no external libraries required).
5. Implement your hash and compare functions per value type.

---

## Testing

Tests can be run using the following commands:

```
bazel test --test_output=all //...
```

## License

This project is released under the **MIT License**.

---

## Author

**[Jeff Manzione](https://github.com/jeffmanzione)**

Formerly at Google — passionate about performant systems, programming languages, and clean C design.

