/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

class Masks {
  final static int DOUBLE_EXP_MASK_1 = 0b10000000000;
  final static int DOUBLE_EXP_MASK_2 = 0b1111111111;
  final static int DOUBLE_EXP_MASK_3 = 0b11111111111;
  final static long DOUBLE_MANTISSA_MASK = 0b1111111111111111111111111111111111111111111111111111L;
}
