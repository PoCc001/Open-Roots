;
; Copyright Johannes Kloimböck 2020.
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE or copy at
; https://www.boost.org/LICENSE_1_0.txt)
;

; Assemble with MASM (Macro Assembler) on a Windows PC with an Intel 64-Bit processor.

.data

INPUT qword ?
GUESS qword ?
HALF_DOUBLE mmword 0.5

.code

osqrt proc
start:
	mov r13, 1024			;move DOUBLE_EXPONENT_MASK1 into r13 for performance reasons
	movq rcx, xmm0			;move input to rcx to enable bit manipulation on it
	movsd xmm3, mmword ptr [HALF_DOUBLE]	;move HALF_DOUBLE into xmm3 for performance reasons
	mov rax, rcx			;copy argument into rax
	shr rax, 52				;get exponent by right-shifting
	jz subnormal_number
	mov rdx, rcx			;copy argument into rdx for use in subnormal_number
	mov r8, rax				;copy exponent into r8
	and rax, r13			;check if the input value is greater than two
	jnz greater_than_two	;conditional jump
	jmp smaller_than_two

greater_than_two:		;if the input value is greater than two do the following instructions
	and r8, 1023		;remove last bit of the exponent
	shr r8, 1			;divide exponent by two
	or r8, r13			;insert last bit of the exponent again
	jmp modified_exp	;go on

smaller_than_two:
	mov r9, r8			;copy exponent
	sub r9, r13			;effectively subtract the (copied) exponent from DOUBLE_EXPONENT_MASK_1
	not r9				;see following 2 instructions
	inc r9
	shr r9, 1			;divide copied exponent by 2
	sub r9, r13			;effectively subtract the (copied) exponent from DOUBLE_EXPONENT_MASK_1
	not r9				;see following 2 instructions
	inc r9
	mov r8, r9			;r8 should hold the exponent
	jmp modified_exp	;go on

subnormal_number:
	mov r12, 1
	shl r12, 53
	dec r12
	and rdx, r12					;extract mantissa
	lzcnt r10, rdx					;count number of leading zeros in mantissa to determine the actual exponent
	mov r11, 11
	sub r10, r11					;subtract 11 (A in hexadecimal) from the leading-zeros-count
	shr r10, 1						;divide this number by 2
	sub r8, r10						;subtract it from the exponent
	jmp modified_exp				;go on

modified_exp:
	shl r8, 52						;construct the first guess of the square root
	mov GUESS, r8
	movsd xmm0, mmword ptr [GUESS]	;move numbers into floating point registers
	mov INPUT, rcx
	movsd xmm1, mmword ptr [INPUT]
	mov r11, 10000000000000H

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
	mulsd xmm0, xmm3

	movsd xmm2, xmm1				;do Newton's iterations #5
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	mulsd xmm0, xmm3

	movsd xmm2, xmm1				;do Newton's iterations #6
	divsd xmm2, xmm0
	addsd xmm0, xmm2
	mulsd xmm0, xmm3

	movsd xmm2, xmm0	;square the guess value and compare it to the input value
	mulsd xmm2, xmm2
	ucomisd xmm2, xmm1	;compare the square of the guess with the input value
	jpe greater			;assume that the guess is only one ulp away from the actual result
	jc smaller

greater:
	movq r8, xmm0
	dec r8								;subtract one ulp
	mov GUESS, r8
	movsd xmm0, mmword ptr [GUESS]		;set return value

smaller:
	movq r8, xmm0
	inc r8								;subtract one ulp
	mov GUESS, r8
	movsd xmm0, mmword ptr [GUESS]		;set return value

	ret					;end the procedure
asmsqrt endp

END		;end of file