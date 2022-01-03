;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The MIT License (MIT)                                                                         ;;
;;                                                                                               ;;
;; Copyright © 2021 - 2022 Johannes Kloimböck                                                    ;;
;;                                                                                               ;;
;; Permission is hereby granted, free of charge, to any person obtaining a copy of this software ;;
;; and associated documentation files (the “Software”), to deal in the Software without          ;;
;; restriction, including without limitation the rights to use, copy, modify, merge, publish,    ;;
;; distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the ;;
;; Software is furnished to do so, subject to the following conditions:                          ;;
;;                                                                                               ;;
;; The above copyright notice and this permission notice shall be included in all copies or      ;;
;; substantial portions of the Software.                                                         ;;
;;                                                                                               ;;
;; THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING ;;
;; BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND    ;;
;; NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  ;;
;; DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,;;
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; IMPORTANT NOTE: Assemble on Windows in MS Visual Studio (MASM) on a SSE4.1-capable x64 Intel or AMD processor!

; These are important values stored in memory that the following procedures will operate with (for scalar and packed instructions)
.data
helper_constants SEGMENT READONLY ALIGN(16)
SIGN_64 qword 2 dup(8000000000000000h)
SIGN_32 dword 4 dup(80000000h)

WITHOUT_SIGN_64 qword 2 dup(7fffffffffffffffh)
WITHOUT_SIGN_32 dword 4 dup(7fffffffh)

DIV_3_64 qword 2 dup(2863311531)
DIV_3_32 dword 4 dup(43691)

ONE_THIRD_64 qword 2 dup(3fd5555555555555h)
ONE_THIRD_32 dword 4 dup(3eaaaaabh)

TWO_THIRDS_64 qword 2 dup(3fe5555555555555h)
TWO_THIRDS_32 dword 4 dup(3f2aaaabh)

FOUR_THIRDS_64 qword 2 dup(3ff5555555555555h)
FOUR_THIRDS_32 dword 4 dup(3faaaaabh)

; There might be better "magical" numbers, but these ones already do a good job.
EXP_MAGIC_MINUEND_64 qword 2 dup(-19178652474277888)
EXP_MAGIC_MINUEND_32 dword 4 dup(4259184641)

FP_INFINITY_64 qword 2 dup(7ff0000000000000h)
FP_INFINITY_32 dword 4 dup(7f800000h)

FP_ONE_64 real8 2 dup(1.0)
FP_ONE_32 real4 4 dup(1.0)

ONES_64 qword 2 dup(18446744073709551615)
ONES_32 dword 4 dup(4294967295)

DIV_3_64_SCALAR qword 5555555500000000h
DIV_3_32_SCALAR dword 5555b700h
helper_constants ENDS

.code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                  INTERNAL MACROS                                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

macro_orcbrt_sd_calc macro
	movq rax, xmm0
	movsd xmm3, xmm0
	mov r9, 8000000000000000h
	mov ecx, 4
	and r9, rax
	xor rax, r9
	mov r8d, 32					; omit this and the following 3 instructions, if you know that no subnormal numbers occur
	mov r10, 7ff0000000000000h
	test rax, r10
	cmovz ecx, r8d
	sub rax, [EXP_MAGIC_MINUEND_64]
	not rax
	mul [DIV_3_64_SCALAR]
	pand xmm3, xmmword ptr [WITHOUT_SIGN_64]
	movq xmm1, rdx
	movsd xmm5, [FOUR_THIRDS_64]
	mulsd xmm3, [ONE_THIRD_64]
	it:
		movsd xmm2, xmm1
		movsd xmm4, xmm3
		mulsd xmm4, xmm1
		mulsd xmm2, xmm1
		mulsd xmm4, xmm2
		subsd xmm4, xmm5
		xorpd xmm4, [SIGN_64]
		mulsd xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_sd_special_cases macro
	movq xmm3, r9
	movsd xmm2, xmm0
	movsd xmm4, [FP_INFINITY_64]
	xorpd xmm5, xmm5
	cmpsd xmm2, xmm4, 4h
	cmpsd xmm0, xmm5, 4h
	andpd xmm1, xmm2
	blendvpd xmm0, xmm1, xmm0
	xorpd xmm0, xmm3
endm


macro_orcbrt_ss_calc macro
	movd eax, xmm0
	movss xmm3, xmm0
	mov r9d, 80000000h
	mov ecx, 3
	and r9d, eax
	xor eax, r9d
	mov r8d, 30					; omit this and the following 2 instructions, if you know that no subnormal numbers occur
	test eax, 7f800000h
	cmovz ecx, r8d
	sub eax, 4259184641
	not eax
	mul dword ptr [DIV_3_32_SCALAR]
	pand xmm3, xmmword ptr [WITHOUT_SIGN_32]
	movd xmm1, edx
	movss xmm5, [FOUR_THIRDS_32]
	mulss xmm3, [ONE_THIRD_32]
	it:
		movss xmm2, xmm1
		movss xmm4, xmm3
		mulss xmm4, xmm1
		mulss xmm2, xmm1
		mulss xmm4, xmm2
		subss xmm4, xmm5
		xorps xmm4, [SIGN_32]
		mulss xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_ss_special_cases macro
	movd xmm3, r9d
	movss xmm2, xmm0
	movss xmm4, [FP_INFINITY_32]
	xorps xmm5, xmm5
	cmpss xmm2, xmm4, 4h
	cmpss xmm0, xmm5, 4h
	andps xmm1, xmm2
	blendvps xmm0, xmm1, xmm0
	xorps xmm0, xmm3
endm


macro_orcbrt_pd_calc macro
	movapd xmm5, xmm0
	pand xmm5, xmmword ptr [SIGN_64]
	pxor xmm0, xmm5
	movapd xmm1, xmm0
	movapd xmm3, xmm0
	psubq xmm1, [EXP_MAGIC_MINUEND_64]
	pxor xmm1, xmmword ptr [ONES_64]
	psrlq xmm1, 33
	pmulhuw xmm1, xmmword ptr [DIV_3_64]
	psllq xmm1, 32
	mulpd xmm3, [ONE_THIRD_64]
	mov ecx, 5				; change to about 32, if you have to deal with denormal numbers (is much slower though)
	it:
		movapd xmm4, xmm3
		movapd xmm2, xmm1
		mulpd xmm4, xmm1
		mulpd xmm2, xmm1
		mulpd xmm4, xmm2
		subpd xmm4, [FOUR_THIRDS_64]
		xorpd xmm4, [SIGN_64]
		mulpd xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_pd_special_cases macro
	movapd xmm2, [FP_INFINITY_64]
	movapd xmm3, xmm0
	xorpd xmm4, xmm4
	cmppd xmm0, xmm4, 4h
	blendvpd xmm2, xmm1, xmm0
	movapd xmm0, [FP_INFINITY_64]
	cmppd xmm0, xmm3, 4h
	andpd xmm0, xmm2
	xorpd xmm0, xmm5
endm


macro_orcbrt_ps_calc macro
	movaps xmm5, xmm0
	pand xmm5, xmmword ptr [SIGN_32]
	pxor xmm0, xmm5
	movaps xmm1, xmm0
	movaps xmm3, xmm0
	psubd xmm1, [EXP_MAGIC_MINUEND_32]
	pxor xmm1, xmmword ptr [ONES_32]
	psrld xmm1, 17
	pmulhuw xmm1, xmmword ptr [DIV_3_32]
	pslld xmm1, 16
	mulps xmm3, [ONE_THIRD_32]
	mov ecx, 3				; change to about 30, if you have to deal with denormal numbers (is much slower though)
	it:
		movaps xmm4, xmm3
		movaps xmm2, xmm1
		mulps xmm4, xmm1
		mulps xmm2, xmm1
		mulps xmm4, xmm2
		subps xmm4, [FOUR_THIRDS_32]
		xorps xmm4, [SIGN_32]
		mulps xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_ps_special_cases macro
	movaps xmm2, [FP_INFINITY_32]
	movaps xmm3, xmm0
	xorps xmm4, xmm4
	cmpps xmm0, xmm4, 4h
	blendvps xmm2, xmm1, xmm0
	movaps xmm0, [FP_INFINITY_32]
	cmpps xmm0, xmm3, 4h
	andps xmm0, xmm1
	xorps xmm0, xmm5
endm


macro_ocbrt_sd_mul macro
	macro_orcbrt_sd_calc
	mulsd xmm1, xmm1
	mulsd xmm0, xmm1
endm

macro_ocbrt_sd_div macro
	macro_orcbrt_sd_calc
	macro_orcbrt_sd_special_cases
	movsd xmm1, [FP_ONE_64]
	divsd xmm1, xmm0
	movsd xmm0, xmm1
endm


macro_ocbrt_ss_mul macro
	macro_orcbrt_ss_calc
	mulss xmm1, xmm1
	mulss xmm0, xmm1
endm

macro_ocbrt_ss_div macro
	macro_orcbrt_ss_calc
	macro_orcbrt_ss_special_cases
	movss xmm1, [FP_ONE_32]
	divss xmm1, xmm0
	movss xmm0, xmm1
endm


macro_ocbrt_pd_mul macro
	macro_orcbrt_pd_calc
	mulpd xmm1, xmm1
	vmulpd xmm0, xmm1
	xorpd xmm0, xmm5
endm

macro_ocbrt_pd_div macro
	macro_orcbrt_pd_calc
	macro_orcbrt_pd_special_cases
	movapd xmm1, [FP_ONE_64]
	divpd xmm1, xmm0
	movapd xmm0, xmm1
endm


macro_ocbrt_ps_mul macro
	macro_orcbrt_ps_calc
	mulps xmm1, xmm1
	mulps xmm0, xmm1
	xorps xmm0, xmm5
endm

macro_ocbrt_ps_div macro
	macro_orcbrt_ps_calc
	macro_orcbrt_ps_special_cases
	movaps xmm1, [FP_ONE_32]
	divps xmm1, xmm0
	movaps xmm0, xmm1
endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                           API MACROS AND PROCEDURES                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Calculates the reciprocal value of the cube root of one double-precision floating-point number.
; Use this macro to inline the code
macro_orcbrt_sd macro
	macro_orcbrt_sd_calc
	macro_orcbrt_sd_special_cases
endm

; Calculates the reciprocal value of the cube root of one double-precision floating-point number.
orcbrt_sd proc
	macro_orcbrt_sd

	ret
orcbrt_sd endp


; Calculates the reciprocal value of the cube root of one single-precision floating-point number.
; Use this macro to inline the code
macro_orcbrt_ss macro
	macro_orcbrt_ss_calc
	macro_orcbrt_ss_special_cases
endm

; Calculates the reciprocal value of the cube root of one single-precision floating-point number.
orcbrt_ss proc
	macro_orcbrt_ss

	ret
orcbrt_ss endp


; Calculates the reciprocal value of the cube root of two double-precision floating-point numbers.
; Use this macro to inline the code
macro_orcbrt_pd macro
	macro_orcbrt_pd_calc
	macro_orcbrt_pd_special_cases
endm

; Calculates the reciprocal value of the cube root of two double-precision floating-point numbers.
orcbrt_pd proc
	macro_orcbrt_pd

	ret
orcbrt_pd endp


; Calculates the reciprocal value of the cube root of four single-precision floating-point numbers.
; Use this macro to inline the code
macro_orcbrt_ps macro
	macro_orcbrt_ps_calc
	macro_orcbrt_ps_special_cases
endm

; Calculates the reciprocal value of the cube root of four single-precision floating-point numbers.
orcbrt_ps proc
	macro_orcbrt_ps

	ret
orcbrt_ps endp


; Calculates the cube root of one double-precision floating-point number.
; Use this macro to inline the code
macro_ocbrt_sd macro
	macro_ocbrt_sd_mul		; change to macro_ocbrt_sd_mul for better performance but more unprecise results
endm

; Calculates the cube root of one double-precision floating-point number.
ocbrt_sd proc
	macro_ocbrt_sd

	ret
ocbrt_sd endp


; Calculates the cube root of one single-precision floating-point number.
; Use this macro to inline the code
macro_ocbrt_ss macro
	macro_ocbrt_ss_mul		; change to macro_ocbrt_ss_mul for better performance but more unprecise results
endm

; Calculates the cube root of one single-precision floating-point number.
ocbrt_ss proc
	macro_ocbrt_ss

	ret
ocbrt_ss endp


; Calculates the cube root of two double-precision floating-point numbers.
; Use this macro to inline the code
macro_ocbrt_pd macro
	macro_ocbrt_pd_div		; change to macro_ocbrt_pd_mul for better performance but more unprecise results
endm

; Calculates the cube root of two double-precision floating-point numbers.
ocbrt_pd proc
	macro_ocbrt_pd

	ret
ocbrt_pd endp


; Calculates the cube root of four single-precision floating-point numbers.
; Use this macro to inline the code
macro_ocbrt_ps macro
	macro_ocbrt_ps_div		; change to macro_ocbrt_ps_mul for better performance but more unprecise results
endm

; Calculates the cube root of four single-precision floating-point numbers.
ocbrt_ps proc
	macro_ocbrt_ps

	ret
ocbrt_ps endp

; Adapted from the famous FISR algorithm
; Use this macro to inline the code
macro_fast_invcbrt_ss macro
	movss xmm2, xmm0
	psubd xmm0, [EXP_MAGIC_MINUEND_32]
	pxor xmm0, xmmword ptr [ONES_32]
	psrld xmm0, 17
	pmulhuw xmm0, xmmword ptr [DIV_3_32]
	pslld xmm0, 16
	mulss xmm2, [ONE_THIRD_32]
	movss xmm1, xmm0
	mulss xmm2, xmm0
	mulss xmm1, xmm0
	mulss xmm2, xmm1
	subss xmm2, [FOUR_THIRDS_32]
	xorps xmm2, xmmword ptr [SIGN_32]
	mulss xmm0, xmm2
endm

; Use this macro to inline the code
macro_fast_cbrt_ss macro
	macro_fast_invcbrt_ss
	rcpss xmm0, xmm0
endm

; Adapted from the famous FISR algorithm
fast_invcbrt_ss proc
	macro_fast_invcbrt_ss

	ret
fast_invcbrt_ss endp

fast_cbrt_ss proc
	macro_fast_cbrt_ss

	ret
fast_cbrt_ss endp

; Adapted from the famous FISR algorithm
; Use this macro to inline the code
macro_fast_invcbrt_ps macro
	movaps xmm2, xmm0
	psubd xmm0, [EXP_MAGIC_MINUEND_32]
	pxor xmm0, xmmword ptr [ONES_32]
	psrld xmm0, 17
	pmulhuw xmm0, xmmword ptr [DIV_3_32]
	pslld xmm0, 16
	mulps xmm2, [ONE_THIRD_32]
	movaps xmm1, xmm0
	mulps xmm2, xmm0
	mulps xmm1, xmm0
	mulps xmm2, xmm1
	subps xmm2, [FOUR_THIRDS_32]
	xorps xmm2, xmmword ptr [SIGN_32]
	mulps xmm0, xmm2
endm

; Use this macro to inline the code
macro_fast_cbrt_ps macro
	macro_fast_invcbrt_ps
	rcpps xmm0, xmm0
endm

; Adapted from the famous FISR algorithm
fast_invcbrt_ps proc
	macro_fast_invcbrt_ps

	ret
fast_invcbrt_ps endp

fast_cbrt_ps proc
	macro_fast_cbrt_ps

	ret
fast_cbrt_ps endp

END