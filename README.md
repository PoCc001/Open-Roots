# Open-Roots
Open-Source implementation of roots (2nd, 3rd etc.) for Java and C/C++ (double datatype)

## API
### Java
The square root and cube root functions can be called in three different ways:
  - calling it from the OSqrt class (OSqrt.sqrt(x))
  - calling it from the ORoots class
    - ORoots.sqrt(x)
    - ORoots.root(x, 2)
The general root of degree n can be computed with ORoots.root(x, n).

### C
Include the oroots.h file. The functions are called csqrt, ccbrt and croot.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling croot(x, 2) or croot(x, 3) respectively.

### C++
Include the oroots.hpp file. The functions are called cppsqrt, cppcbrt and cpproot.
The first two take a double variable as an input. The third one takes a double and an int.
The square root and the cube root can also be computed by calling cpproot(x, 2) or cpproot(x, 3) respectively.

**Note:** x is a variable of the primitive datatype double.
