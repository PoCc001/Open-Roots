;
; Copyright Johannes Kloimböck 2020.
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE or copy at
; https://www.boost.org/LICENSE_1_0.txt)
;

; Assemble with MASM (Macro Assembler) on a Windows PC with an Intel or AMD 64-Bit processor that is
; SSE-capable.

.data	;empty

.code

osqrt proc
start:

	movq rcx, xmm0			;move input to rcx to enable bit manipulation on it
	movsd xmm5, xmm0		;copy the input value into xmm5

	mov rax, rcx			;copy argument into rax
	mov r11, 1
	mov rdx, rcx			;copy argument into rdx for use in subnormal_number

	mov r13, 3fe0000000000000h
	movq xmm3, r13			;move 0.5 into xmm3
	xor r12, r12
	shr rax, 52				;get exponent by right-shifting
	cmovz r11, r12
	mov r8, rax				;copy exponent into r8

	sub r8, 1024			;subtract 2^10 from the exponent
	sar r8, 1				;divide it by 2
	add r8, 1024			;add 2^10 again
	and r11, r11			;test the exponent of the input value, if it is 0 or not
	jnz modified_exp

subnormal_number:
	mov r12, 1fffffffffffffh		;bitmask for mantissa
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
	mov rcx, 10000000000000h

	movsd xmm2, xmm1				;do Newton's iterations #1
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	movq rax, xmm0
	sub rax, rcx
	movq xmm0, rax

	movsd xmm2, xmm1				;do Newton's iterations #2
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	movq rax, xmm0
	sub rax, rcx
	movq xmm0, rax
	
	movsd xmm2, xmm1				;do Newton's iterations #3
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	movq rax, xmm0
	sub rax, rcx
	movq xmm0, rax

	movsd xmm2, xmm1				;do Newton's iterations #4
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	movsd xmm6, xmm0				;sqrt times 2 saved in xmm6 register
	movq rax, xmm0
	sub rax, rcx
	movq xmm0, rax

	movsd xmm2, xmm0	;do the multiplication and subtraction in "extended precision"

	vfmsub213sd xmm2, xmm2, xmm1
	divsd xmm2, xmm6

	subsd xmm0, xmm2

	movq rdx, xmm0
	mov rcx, 7FFFFFFFFFFFFFFFh
	mov r8, 0h

	xorpd xmm1, xmm1
	ucomisd xmm5, xmm1
	cmovnc rcx, rdx			;decide, whether the output value should be the algorithmically calculated one, 0.0 or NaN
	cmovz rcx, r8

	movq xmm0, rcx
	
	ret					;end the procedure
asmsqrt endp

END		;end of file