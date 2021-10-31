;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The MIT License (MIT)                                                                         ;;
;;                                                                                               ;;
;; Copyright © 2021 Johannes Kloimböck                                                           ;;
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

; IMPORTANT NOTE: Assemble on Windows in MS Visual Studio (MASM) on a AVX2- and FMA3-capable x64 Intel or AMD processor!

; These are important values stored in memory that the following procedures will operate with (for scalar and packed instructions)
.data
SIGN_64 qword 4 dup(8000000000000000h)
SIGN_32 dword 8 dup(80000000h)

WITHOUT_SIGN_64 qword 4 dup(7fffffffffffffffh)
WITHOUT_SIGN_32 dword 8 dup(7fffffffh)

; There might be better "magical" numbers, but these ones already do a good job.
EXP_MAGIC_ADDEND_64 qword 4 dup(2a9f5cc62cb0f9e1h)
EXP_MAGIC_ADDEND_32 dword 8 dup(2a501a5bh)

DIV_3_64 qword 4 dup(2863311531)
DIV_3_32 dword 8 dup(43691)

ONE_THIRD_64 qword 4 dup(3fd5555555555555h)
ONE_THIRD_32 dword 8 dup(3eaaaaabh)

TWO_THIRDS_64 qword 4 dup(3fe5555555555555h)
TWO_THIRDS_32 dword 8 dup(3f2aaaabh)

FOUR_THIRDS_64 qword 4 dup(3ff5555555555555h)
FOUR_THIRDS_32 dword 8 dup(3faaaaabh)

EXP_MASK_64 qword 4 dup(7ff0000000000000h)
EXP_MASK_32 dword 8 dup(7f800000h)

; There might be better "magical" numbers, but these ones already do a good job.
EXP_MAGIC_MINUEND_64 qword 4 dup(-19178652474277888)
EXP_MAGIC_MINUEND_32 dword 8 dup(4259184641)

FP_INFINITY_64 qword 4 dup(7ff0000000000000h)
FP_INFINITY_32 dword 8 dup(7f800000h)

FP_ONE_64 real8 4 dup(1.0)
FP_ONE_32 real4 8 dup(1.0)

ONES_64 qword 4 dup(18446744073709551615)
ONES_32 dword 8 dup(4294967295)

DIV_3_64_SCALAR qword 5555555500000000h
DIV_3_32_SCALAR dword 5555b700h

.code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                  INTERNAL MACROS                                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
macro_orcbrt_sd_calc macro
	vmovq rax, xmm0
	mov r9, 8000000000000000h
	mov r10, [EXP_MASK_64]
	mov ecx, 4
	and r9, rax
	xor rax, r9
	mov r8d, 32					; omit this and the following 2 instructions, if you know that no subnormal numbers occur
	test rax, r10
	cmovz ecx, r8d
	sub rax, [EXP_MAGIC_MINUEND_64]
	not rax
	mul [DIV_3_64_SCALAR]
	vpand xmm0, xmm0, [WITHOUT_SIGN_64]
	vmovq xmm1, rdx
	vmovsd xmm5, [FOUR_THIRDS_64]
	vmulsd xmm3, xmm0, [ONE_THIRD_64]
	it:
		vmulsd xmm4, xmm3, xmm1
		vmulsd xmm2, xmm1, xmm1
		vfnmadd213sd xmm4, xmm2, xmm5
		vmulsd xmm1, xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_sd_special_cases macro
	vmovsd xmm4, [FP_INFINITY_64]
	vxorpd xmm5, xmm5, xmm5
	vcmpsd xmm2, xmm0, xmm4, 4h
	vcmpsd xmm3, xmm0, xmm5, 4h
	vandpd xmm1, xmm1, xmm2
	vmovq xmm2, r9
	vblendvpd xmm1, xmm4, xmm1, xmm3
	vpxor xmm0, xmm2, xmm1
endm


macro_orcbrt_ss_calc macro
	vmovd eax, xmm0
	mov r9d, 80000000h
	mov r10d, [EXP_MASK_32]
	mov ecx, 3
	and r9d, eax
	xor eax, r9d
	mov r8d, 30					; omit this and the following 2 instructions, if you know that no subnormal numbers occur
	test eax, r10d
	cmovz ecx, r8d
	sub eax, [EXP_MAGIC_MINUEND_32]
	not eax
	mul dword ptr [DIV_3_32_SCALAR]
	vpand xmm0, xmm0, dword ptr [WITHOUT_SIGN_32]
	vmovd xmm1, edx
	vmovss xmm5, [FOUR_THIRDS_32]
	vmulss xmm3, xmm0, [ONE_THIRD_32]
	it:
		vmulss xmm4, xmm3, xmm1
		vmulss xmm2, xmm1, xmm1
		vfnmadd213ss xmm4, xmm2, xmm5
		vmulss xmm1, xmm1, xmm4
		dec ecx
		jnz it
endm

macro_orcbrt_ss_special_cases macro
	vmovss xmm4, [FP_INFINITY_32]
	vxorps xmm5, xmm5, xmm5
	vcmpss xmm2, xmm0, xmm4, 4h
	vcmpss xmm3, xmm0, xmm5, 4h
	vandps xmm1, xmm1, xmm2
	vmovd xmm2, r9d
	vblendvps xmm1, xmm4, xmm1, xmm3
	vpxor xmm0, xmm2, xmm1
endm


macro_orcbrt_pd_calc macro
	vpand ymm5, ymm0, [SIGN_64]
	vpxor ymm0, ymm0, ymm5
	vpsubq ymm1, ymm0, [EXP_MAGIC_MINUEND_64]
	vpxor ymm1, ymm1, [ONES_64]
	vpsrlq ymm1, ymm1, 33
	vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_64]
	vpsllq ymm1, ymm1, 32
	vmulpd ymm3, ymm0, [ONE_THIRD_64]
	mov ecx, 5				; change to about 32, if you have to deal with denormal numbers (is much slower though)
	it:
		vmulpd ymm4, ymm3, ymm1
		vmulpd ymm2, ymm1, ymm1
		vfnmadd213pd ymm4, ymm2, [FOUR_THIRDS_64]
		vmulpd ymm1, ymm1, ymm4
		dec ecx
		jnz it
endm

macro_orcbrt_pd_special_cases macro
	vxorpd ymm4, ymm4, ymm4
	vcmppd ymm3, ymm0, ymm4, 4h
	vmovapd ymm4, [FP_INFINITY_64]
	vblendvpd ymm1, ymm4, ymm1, ymm3
	vcmppd ymm2, ymm0, ymm4, 4h
	vandpd ymm1, ymm1, ymm2
	vxorpd ymm0, ymm1, ymm5
endm


macro_orcbrt_ps_calc macro
	vpand ymm5, ymm0, [SIGN_32]
	vpxor ymm0, ymm0, ymm5
	vpsubd ymm1, ymm0, [EXP_MAGIC_MINUEND_32]
	vpxor ymm1, ymm1, [ONES_32]
	vpsrld ymm1, ymm1, 17
	vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_32]
	vpslld ymm1, ymm1, 16
	vmulps ymm3, ymm0, [ONE_THIRD_32]
	mov ecx, 3			; change to about 30, if you have to deal with denormal numbers (is much slower though)
	it:
		vmulps ymm4, ymm3, ymm1
		vmulps ymm2, ymm1, ymm1
		vfnmadd213ps ymm4, ymm2, [FOUR_THIRDS_32]
		vmulps ymm1, ymm1, ymm4
		dec ecx
		jnz it
endm

macro_orcbrt_ps_special_cases macro
	vxorps ymm4, ymm4, ymm4
	vcmpps ymm3, ymm0, ymm4, 4h
	vmovaps ymm4, [FP_INFINITY_32]
	vblendvps ymm1, ymm4, ymm1, ymm3
	vcmpps ymm2, ymm0, ymm4, 4h
	vandps ymm1, ymm1, ymm2
	vxorps ymm0, ymm1, ymm5
endm


macro_ocbrt_sd_mul macro
	macro_orcbrt_sd_calc
	vmovq xmm2, r9
	vmulsd xmm1, xmm1, xmm1
	vmulsd xmm0, xmm1, xmm0
	vxorpd xmm0, xmm0, xmm2
endm

macro_ocbrt_sd_div macro
	macro_orcbrt_sd_calc
	macro_orcbrt_sd_special_cases
	vmovsd xmm1, [FP_ONE_64]
	vdivsd xmm0, xmm1, xmm0
endm

macro_ocbrt_ss_mul macro
	macro_orcbrt_ss_calc
	vmovd xmm2, r9d
	vmulss xmm1, xmm1, xmm1
	vmulss xmm0, xmm1, xmm0
	vxorps xmm0, xmm0, xmm2
endm

macro_ocbrt_ss_div macro
	macro_orcbrt_ss_calc
	macro_orcbrt_ss_special_cases
	vmovss xmm1, [FP_ONE_32]
	vdivss xmm0, xmm1, xmm0
endm


macro_ocbrt_pd_mul macro
	macro_orcbrt_pd_calc
	vmulpd ymm1, ymm1, ymm1
	vmulpd ymm0, ymm1, ymm0
	vxorpd ymm0, ymm0, ymm5
endm

macro_ocbrt_pd_div macro
	macro_orcbrt_pd_calc
	macro_orcbrt_pd_special_cases
	vmovapd ymm1, [FP_ONE_64]
	vdivpd ymm0, ymm1, ymm0
endm


macro_ocbrt_ps_mul macro
	macro_orcbrt_ps_calc
	vmulps ymm1, ymm1, ymm1
	vmulps ymm0, ymm1, ymm0
	vxorps ymm0, ymm0, ymm5
endm

macro_ocbrt_ps_div macro
	macro_orcbrt_ps_calc
	macro_orcbrt_ps_special_cases
	vmovaps ymm1, [FP_ONE_32]
	vdivps ymm0, ymm1, ymm0
endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                           API MACROS AND PROCEDURES                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Calculates the reciprocal value of the cube root of one double-precision floating-point number.
; Use this macro to inline the code
macro_orcbrt_sd macro
	macro_orcbrt_sd_calc
	macro_orcbrt_sd_special_cases
	;	vzeroupper			; Uncomment this instruction, if your software contains SSE instructions directly after this macro.
							; If you're unsure, read through the disassembly and decide based on that or uncomment it anyway.
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
	;	vzeroupper			; Uncomment this instruction, if your software contains SSE instructions directly after this macro.
							; If you're unsure, read through the disassembly and decide based on that or uncomment it anyway.
endm

; Calculates the reciprocal value of the cube root of one single-precision floating-point number.
orcbrt_ss proc
	macro_orcbrt_ss

	ret
orcbrt_ss endp


; Calculates the reciprocal value of the cube root of four double-precision floating-point numbers.
; Use this macro to inline the code
macro_orcbrt_pd macro
	macro_orcbrt_pd_calc
	macro_orcbrt_pd_special_cases
endm

; Calculates the reciprocal value of the cube root of four double-precision floating-point numbers.
orcbrt_pd proc
	macro_orcbrt_pd

	ret
orcbrt_pd endp


; Calculates the reciprocal value of the cube root of eight single-precision floating-point numbers.
; Use this macro to inline the code
macro_orcbrt_ps macro
	macro_orcbrt_ps_calc
	macro_orcbrt_ps_special_cases
endm

; Calculates the reciprocal value of the cube root of eight single-precision floating-point numbers.
orcbrt_ps proc
	macro_orcbrt_ps

	ret
orcbrt_ps endp


; Calculates the cube root of one double-precision floating-point number.
; Use this macro to inline the code
macro_ocbrt_sd macro
	macro_ocbrt_sd_div		; change to macro_ocbrt_sd_mul for better performance but more unprecise results
	;	vzeroupper			; Uncomment this instruction, if your software contains SSE instructions directly after this macro.
							; If you're unsure, read through the disassembly and decide based on that or uncomment it anyway.
endm

; Calculates the cube root of one double-precision floating-point number.
ocbrt_sd proc
	macro_ocbrt_sd

	ret
ocbrt_sd endp


; Calculates the cube root of one single-precision floating-point number.
; Use this macro to inline the code
macro_ocbrt_ss macro
	macro_ocbrt_ss_div		; change to macro_ocbrt_ss_mul for better performance but more unprecise results
	;	vzeroupper			; Uncomment this instruction, if your software contains SSE instructions directly after this macro.
							; If you're unsure, read through the disassembly and decide based on that or uncomment it anyway.
endm

; Calculates the cube root of one single-precision floating-point number.
ocbrt_ss proc
	macro_ocbrt_ss

	ret
ocbrt_ss endp


; Calculates the cube root of four double-precision floating-point numbers.
; Use this macro to inline the code
macro_ocbrt_pd macro
	macro_ocbrt_pd_div		; change to macro_ocbrt_pd_mul for better performance but more unprecise results
endm

; Calculates the cube root of four double-precision floating-point numbers.
ocbrt_pd proc
	macro_ocbrt_pd

	ret
ocbrt_pd endp


; Calculates the cube root of one single-precision floating-point number.
; Use this macro to inline the code
macro_ocbrt_ps macro
	macro_ocbrt_ps_div		; change to macro_ocbrt_ps_mul for better performance but more unprecise results
endm

; Calculates the cube root of eight single-precision floating-point numbers.
ocbrt_ps proc
	macro_ocbrt_ps

	ret
ocbrt_ps endp

; Adapted from the famous FISR algorithm
; Use this macro to inline the code
macro_fast_invcbrt_ss macro
	vpsubd xmm1, xmm0, [EXP_MAGIC_MINUEND_32]
	vpxor xmm1, xmm1, [ONES_32]
	vpsrld xmm1, xmm1, 17
	vpmulhuw xmm1, xmm1, xmmword ptr [DIV_3_32]
	vpslld xmm1, xmm1, 16
	vmulss xmm3, xmm0, [ONE_THIRD_32]
	vmulss xmm4, xmm3, xmm1
	vmulss xmm2, xmm1, xmm1
	vfnmadd213ss xmm4, xmm2, [FOUR_THIRDS_32]
	vmulss xmm0, xmm1, xmm4
endm

; Use this macro to inline the code
macro_fast_cbrt_ss macro
	macro_fast_invcbrt_ss
	vrcpss xmm0, xmm0, xmm0
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
	vpsubd ymm1, ymm0, [EXP_MAGIC_MINUEND_32]
	vpxor ymm1, ymm1, [ONES_32]
	vpsrld ymm1, ymm1, 17
	vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_32]
	vpslld ymm1, ymm1, 16
	vmulps ymm3, ymm0, [ONE_THIRD_32]
	vmulps ymm4, ymm3, ymm1
	vmulps ymm2, ymm1, ymm1
	vfnmadd213ps ymm4, ymm2, [FOUR_THIRDS_32]
	vmulps ymm0, ymm1, ymm4
endm

; Use this macro to inline the code
macro_fast_cbrt_ps macro
	macro_fast_invcbrt_ps
	vrcpps ymm0, ymm0
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
