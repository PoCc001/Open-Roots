;
; Copyright Johannes Kloimböck 2020.
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE or copy at
; https://www.boost.org/LICENSE_1_0.txt)
;

; Assemble with MASM (Macro Assembler) on a Windows PC with an Intel 64-Bit processor.

.data

HALF_DOUBLE mmword +0.5

.code

asmsqrt proc
start:
	xor rax, rax
	movq xmm1, rax
	ucomisd xmm0, xmm1		;check the sign of the input
	jz zero
	jnc positive
	jmp not_positive

positive:
	mov r13, 1024			;move DOUBLE_EXPONENT_MASK1 into r13 for performance reasons
	movq rcx, xmm0			;move input to rcx to enable bit manipulation on it
	movsd xmm3, mmword ptr [HALF_DOUBLE]	;move HALF_DOUBLE into xmm3 for performance reasons
	mov rax, rcx			;copy argument into rax
	xor r12, r12
	shr rax, 52				;get exponent by right-shifting
	cmovz r11, r12
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
	neg r9
	shr r9, 1			;divide copied exponent by 2
	sub r9, r13			;effectively subtract the (copied) exponent from DOUBLE_EXPONENT_MASK_1
	neg r9
	mov r8, r9			;r8 should hold the exponent
	and r11, r11
	jz subnormal_number
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
	shr r12, 42
	and r8, r12
	jmp modified_exp				;go on

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

	xor r9, r9			;set up the sign of the ulp to add to the guess
	mov r10, r9
	inc r10
	mov r11, r9
	not r11

	ucomisd xmm2, xmm1	;compare the square of the guess with the input value

	cmovnc r12, r11
	cmovc r12, r10
	cmovz r12, r9

	movq r8, xmm0

	add r8, r12			;correct the guess

	movq xmm0, r8
	ret				;end the procedure

not_positive:		;if the input is negative, insert NaN into xmm0, else return 0.0
	xor r8, r8		;make the value in rax 0
	inc r8			;build the NaN value
	shl r8, 63
	not r8
	movq xmm0, r8
	ret					;end the procedure

zero:
	
	ret					;end the procedure
asmsqrt endp

END		;end of file