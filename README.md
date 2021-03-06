# Open-Roots
Open-Source implementation of roots (2nd, 3rd etc.) for Java and C/C++ (double datatype) which is
optimized for speed rather than 100% precise results. This means that results may vary one ulp from the actual
value and be incorrectly rounded. Instead, this library aims to provide an open-source implementation of mathematical
roots which is faster than other platform indipendent libaries of the same kind.

## Note
This library assumes that the *double* datatype in C/C++ is 64 Bit wide. Also, a *double* must fit into an
*unsigned long long*.

## API
### Java
The square root and cube root functions can be called in three different ways:
  - calling it from the OSqrt class (OSqrt.sqrt(x))
  - calling it from the ORoots class
    - ORoots.sqrt(x)
    - ORoots.root(x, 2)
The general root of degree n can be computed with ORoots.root(x, n).
All the necessary files are located in Java/oroots directory.

### C
Include the oroots.h file which can be found in the "C" directory. The functions are called osqrt, ocbrt and oroot.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling oroot(x, 2) or oroot(x, 3) respectively.

If you know that the root functions won't have to deal with special values like 0, NaN or subnormal numbers, you can turn
of checking for them at compile time. Doing so will most likely increase the performance of the functions in this library.
This can be done by defining the macro(s) *CHECK_SPECIAL_CASES* and/or *SUBNORMAL_NUMBERS* as 0.
YOU SHOULD ONLY DO SO IF YOU REALLY KNOW WHAT YOU ARE DOING!

### C++
Include the oroots.hpp file which can be found in the "C" directory. The functions are called the same.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling oroot(x, 2) or oroot(x, 3) respectively.

If you know that the root functions won't have to deal with special values like 0, NaN or subnormal numbers, you can turn
of checking for them at compile time. Doing so will most likely increase the performance of the functions in this library.
This can be done by defining the macro(s) *CHECK_SPECIAL_CASES* and/or *SUBNORMAL_NUMBERS* as 0.
YOU SHOULD ONLY DO SO IF YOU REALLY KNOW WHAT YOU ARE DOING!

**Note:** x is a variable of the primitive datatype *double* in Java and C/C++.