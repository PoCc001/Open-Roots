# Open-Roots
Open-Source implementation of roots (2nd, 3rd etc.) for Java and C/C++ (double datatype) which is optimized for
speed rather than 100% precise results. This means that results may vary one ulp from the actual value and be
incorrectly rounded. Instead, this library aims to provide an open-source implementation of mathematical roots
which is faster than other platform indipendent libaries of the same kind.

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
Include the oroots.h file which can be found in the "C" directory. The functions are called csqrt, ccbrt and croot.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling croot(x, 2) or croot(x, 3) respectively.

### C++
Include the oroots.hpp file which can be found in the "C" directory. The functions are called cppsqrt, cppcbrt and cpproot.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling cpproot(x, 2) or cpproot(x, 3) respectively.

### x86/64 Assembly
Call the procedure osqrt found in the file "osqrt.asm" in the "Assembly" directory. The floating point argument has to be
stored in the xmm0 register. The result will be moved into the same register. "osqrt.asm" should be assembled using the
Macro Assembler (MASM). It can also be used with C/C++ projects in Visual Studio.

**Note:** x is a variable of the primitive datatype double.
