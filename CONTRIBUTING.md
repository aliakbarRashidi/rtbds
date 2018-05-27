# Contributing to RTBDS

RTBDS follows [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), with the following exceptions:
* Use `camelCase` for all functions, including accessors and mutators
* Use `camelCase` for local and static variables, as well as function parameters
* Use `m_camelCase` for member variables
* Use `g_camelCase` for global variables
* Use `UPPER_CASE` for constants
* Use `PascalCase` for enumerations members
* Use `#pragma once` instead of standard include guards
* Use `.cpp` extension for source files, `.h` for header files
* Use **smart pointers**, raw pointers are forbidden, except when there is no other way (e.g. legacy API)
* Use **uniform initialization syntax**, except when it is not possible (e.g. constructor expecting an initializer list)
* Use **tabs** for indentation, one tab per indentation level
* **Enclose code blocks with braces** `{}`, even one-line blocks
* There is no **line length limit**, however stay reasonable
* Do *not* indent **access modifiers** (`public`, `protected`, `private`) in classes
* *Do* indent **initialization lists**, one element per line, the parent class constructor on the same line as the name of the current class constructor

Above all, **be** ***consistent*** **within your code**!
