# Open-Roots
Open-Source implementation of roots (2nd, 3rd etc.) for Java and C/C++ (double datatype) which is
optimized for speed rather than 100% precise results. This means that results may vary one ulp from the actual
value and be incorrectly rounded. Instead, this library aims to provide an open-source implementation of mathematical
roots which is faster than other platform indipendent libaries of the same kind.

## Note
This library assumes that the *double* datatype in C/C++ is 64 Bit wide. Also, a *double* must fit into an
*unsigned long long*. The Java and C/C++ version of Open-Roots, as well as the benchmark programs, are licensed under the Boost Software License v1.0. The assembly version
however is licensed under the MIT license.

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
Setting *ONLY_USE_RECIP_ROOTS* to 1 may allow for faster calculation of osqrt(f) and ocbrt(f) at the cost of less accuracy.

### C++
Include the oroots.hpp file which can be found in the "C" directory. The functions are called the same. However, the namespace
*oroots* contains wrapper functions for the aforementioned ones that are just called sqrt, cbrt and root.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling oroot(x, 2) or oroot(x, 3) respectively.

If you know that the root functions won't have to deal with special values like 0, NaN or subnormal numbers, you can turn
of checking for them at compile time. Doing so will most likely increase the performance of the functions in this library.
This can be done by defining the macro(s) *CHECK_SPECIAL_CASES* and/or *SUBNORMAL_NUMBERS* as 0.
YOU SHOULD ONLY DO SO IF YOU REALLY KNOW WHAT YOU ARE DOING!
Setting *ONLY_USE_RECIP_ROOTS* to 1 may allow for faster calculation of osqrt(f) and ocbrt(f) at the cost of less accuracy.

**Note:** x is a variable of the primitive datatype *double* or *float* in Java and C/C++.

### x64 Assembly
Assemble the asmcbrt.asm file in Visual Studio using the Macro Asssembler on an AVX2- and FMA-capable machine. For maximum
performance, one could consider optimizing the code for a specific processor model. There are only procedures for calculating
the cube root and its reciprocal, as the square root can be computed via a single assembly instruction. Maybe, the generic root
function will be available in the future. Currently, no AVX-512 instructions are used.
