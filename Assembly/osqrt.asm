;
; Copyright Johannes Kloimböck 2020.
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE or copy at
; https://www.boost.org/LICENSE_1_0.txt)
;

; Assemble with MASM (Macro Assembler) on a Windows PC with an Intel or AMD 64-Bit processor that is
; SSE-capable.

.data

HALF_DOUBLE mmword +0.5

.code

osqrt proc
start:
	xorpd xmm1, xmm1

	movq rcx, xmm0			;move input to rcx to enable bit manipulation on it

	ucomisd xmm0, xmm1		;check the sign of the input
	jz zero
	jnc positive
	jmp not_positive

positive:
	mov r13, 1024			;move DOUBLE_EXPONENT_MASK1 into r13 for performance reasons
	movsd xmm3, mmword ptr [HALF_DOUBLE]	;move HALF_DOUBLE into xmm3 for performance reasons
	mov rax, rcx			;copy argument into rax
	xor r12, r12
	mov r11, 1
	shr rax, 52				;get exponent by right-shifting
	cmovz r11, r12
	mov rdx, rcx			;copy argument into rdx for use in subnormal_number
	mov r8, rax				;copy exponent into r8

	sub r8, 1024			;subtract 2^10 from the exponent
	sar r8, 1				;divide it by 2
	add r8, 1024			;add 2^10 again
	and r11, r11
	jnz modified_exp

subnormal_number:
	inc r12
	shl r12, 53
	dec r12
	and rdx, r12					;extract mantissa
	lzcnt r10, rdx					;count number of leading zeros in mantissa to determine the actual exponent
	mov r11, 11
	sub r10, r11					;subtract 11 (B in hexadecimal) from the leading-zeros-count
	sar r10, 1						;divide this number by 2
	sub r8, r10						;subtract it from the exponent
	shr r12, 42
	and r8, r12

modified_exp:
	shl r8, 52						;construct the first guess of the square root
	movq xmm0, r8					;move numbers into floating point registers
	movq xmm1, rcx

	movsd xmm2, xmm1				;do Newton's iterations #1
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	mulsd xmm0, xmm3

	movsd xmm2, xmm1				;do Newton's iterations #2
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	mulsd xmm0, xmm3

	movsd xmm2, xmm1				;do Newton's iterations #3
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	mulsd xmm0, xmm3

	movsd xmm2, xmm1				;do Newton's iterations #4
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	movsd xmm6, xmm0				;sqrt times 2 saved in xmm6 register
	mulsd xmm0, xmm3

	movsd xmm2, xmm0				;do bit correction

	vfmsub213sd xmm2, xmm2, xmm1
	divsd xmm2, xmm6

	subsd xmm0, xmm2
	
	ret				;end the procedure

not_positive:		;if the input is negative, insert NaN into xmm0, else return 0.0
	mov rax, 7FFFFFFFFFFFFFFFh
	movq xmm0, rax
	ret					;end the procedure

zero:
	
	ret					;end the procedure
asmsqrt endp

END		;end of file