;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The MIT License (MIT)                                                                         ;;
;;                                                                                               ;;
;; Copyright © 2021 Johannes Kloimböck                                                           ;;
;;                                                                                               ;;
;; Permission is hereby granted, free of charge, to any person obtaining a copy of this software ;;
;; and associated documentation files (the “Software”), to deal in the Software without          ;;
;; restriction, including without limitation the rights to use, copy, modify, merge, publish,	 ;;
;; distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the ;;
;; Software is furnished to do so, subject to the following conditions:                          ;;
;;                                                                                               ;;
;; The above copyright notice and this permission notice shall be included in all copies or      ;;
;; substantial portions of the Software.                                                         ;;
;;                                                                                               ;;
;; THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING ;;
;; BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND	 ;;
;; NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,	 ;;
;; DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,;;
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; IMPORTANT NOTE: Assemble on Windows in MS Visual Studio (MASM) on a AVX2- and FMA3-capable machine!

; These are important values stored in memory that the following procedures will operate with (for scalar and packed instructions)
.data
SIGN_64 qword 8000000000000000h, 8000000000000000h, 8000000000000000h, 8000000000000000h
SIGN_32 dword 80000000h, 80000000h, 80000000h, 80000000h, 80000000h, 80000000h, 80000000h, 80000000h

EXP_ADDEND_64 qword 2ab0000000000000h, 2ab0000000000000h, 2ab0000000000000h, 2ab0000000000000h
EXP_ADDEND_32 dword 2a800000h, 2a800000h, 2a800000h, 2a800000h, 2a800000h, 2a800000h, 2a800000h, 2a800000h

DIV_3_64 qword 2863311531, 2863311531, 2863311531, 2863311531
DIV_3_32 dword 43691, 43691, 43691, 43691, 43691, 43691, 43691, 43691

ONE_THIRD_64 qword 3fd5555555555555h, 3fd5555555555555h, 3fd5555555555555h, 3fd5555555555555h
ONE_THIRD_32 dword 3eaaaaabh, 3eaaaaabh, 3eaaaaabh, 3eaaaaabh, 3eaaaaabh, 3eaaaaabh, 3eaaaaabh, 3eaaaaabh

TWO_THIRDS_64 qword 3fe5555555555555h, 3fe5555555555555h, 3fe5555555555555h, 3fe5555555555555h
TWO_THIRDS_32 dword 3f2aaaabh, 3f2aaaabh, 3f2aaaabh, 3f2aaaabh, 3f2aaaabh, 3f2aaaabh, 3f2aaaabh, 3f2aaaabh

FOUR_THIRDS_64 qword 3ff5555555555555h, 3ff5555555555555h, 3ff5555555555555h, 3ff5555555555555h
FOUR_THIRDS_32 dword 3faaaaabh, 3faaaaabh, 3faaaaabh, 3faaaaabh, 3faaaaabh, 3faaaaabh, 3faaaaabh, 3faaaaabh

EXP_MASK_64 qword 7ff0000000000000h, 7ff0000000000000h, 7ff0000000000000h, 7ff0000000000000h
EXP_MASK_32 dword 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h

EXP_MINUEND_64 qword -13510798882111488, -13510798882111488, -13510798882111488, -13510798882111488
EXP_MINUEND_32 dword 4261412864, 4261412864, 4261412864, 4261412864, 4261412864, 4261412864, 4261412864, 4261412864

; There might be a better "magical" number, but this one already does a good job.
MAGICAL_NUMBER dword 4259184641, 4259184641, 4259184641, 4259184641, 4259184641, 4259184641, 4259184641, 4259184641

FP_INFINITY_64 qword 7ff0000000000000h, 7ff0000000000000h, 7ff0000000000000h, 7ff0000000000000h
FP_INFINITY_32 dword 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h, 7f800000h

ONES_64 qword -1, -1, -1, -1
ONES_32 dword -1, -1, -1, -1, -1, -1, -1, -1

DIV_3_64_SCALAR qword 5555555500000000h
DIV_3_32_SCALAR dword 5555b700h

.code
; Calculates the cube root of one double-precision floating-point number.
ocbrt_sd proc
	start:
		vmovq rax, xmm0
		vxorpd xmm2, xmm2, xmm2
		mov r9, 8000000000000000h
		and r9, rax
		xor rax, r9
		mov ecx, 7
		mov r8d, 35				; omit this and the following 2 instructions, if you know that no subnormal numbers occur
		test rax, [EXP_MASK_64]
		cmovz ecx, r8d
		mul [DIV_3_64_SCALAR]
		add rdx, [EXP_ADDEND_64]
		vcmpsd xmm4, xmm0, xmm2, 4h

	newton_iterations:
		vmovq xmm1, rdx
		vmovsd xmm2, [TWO_THIRDS_64]
		vmulsd xmm0, xmm0, [ONE_THIRD_64]

		it:
			vmulsd xmm3, xmm1, xmm1
			vdivsd xmm3, xmm0, xmm3
			vfmadd213sd xmm1, xmm2, xmm3

			dec ecx
			jnz it

		vmovq xmm0, r9
		vandpd xmm1, xmm1, xmm4
		vxorpd xmm0, xmm0, xmm1

	ret
ocbrt_sd endp

; Calculates the cube root of four double-precision floating-point numbers.
ocbrt_pd proc
	start:
		vpand ymm2, ymm0, [SIGN_64]
		vpxor ymm0, ymm2, ymm0
		vpxor ymm4, ymm4, ymm4
		vpsrlq ymm1, ymm0, 33
		vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_64]
		vpsllq ymm1, ymm1, 32
		vpaddq ymm1, ymm1, [EXP_ADDEND_64]
		vcmppd ymm3, ymm0, ymm4, 4h
		vandpd ymm1, ymm1, ymm3

	newton_iterations:
		vmovapd ymm5, [TWO_THIRDS_64]
		vmovapd ymm4, [ONE_THIRD_64]
		vmulpd ymm0, ymm0, ymm4
		mov ecx, 7					; change to about 35, if you have to deal with denormal numbers (is much slower though)

		it:
			vmulpd ymm3, ymm1, ymm1
			vdivpd ymm3, ymm0, ymm3
			vfmadd213pd ymm1, ymm5, ymm3
			dec ecx
			jnz it

		vxorpd xmm0, xmm1, xmm2

	ret
ocbrt_pd endp

; Calculates the cube root of one single-precision floating-point number.
ocbrt_ss proc
	start:
		vmovd eax, xmm0
		vxorps xmm2, xmm2, xmm2
		mov r9d, 80000000h
		and r9d, eax
		xor eax, r9d
		mov ecx, 4
		mov r8d, 32					; omit this and the following 2 instructions, if you know that no subnormal numbers occur
		test eax, [EXP_MASK_32]
		cmovz ecx, r8d
		mul [DIV_3_32_SCALAR]
		add edx, [EXP_ADDEND_32]
		vcmpss xmm4, xmm0, xmm2, 4h

	newton_iterations:
		vmovd xmm1, edx
		vmovss xmm2, [ONE_THIRD_32]
		vmovss xmm5, [TWO_THIRDS_32]
		vmulss xmm0, xmm0, xmm2

		it:
			vmulss xmm3, xmm1, xmm1
			vdivss xmm3, xmm0, xmm3
			vfmadd213ss xmm1, xmm5, xmm3
			dec ecx
			jnz it

		vmovd xmm0, r9d
		vandps xmm1, xmm1, xmm4
		vxorps xmm0, xmm0, xmm1
		
	ret
ocbrt_ss endp

; Calculates the cube root of eight single-precision floating-point numbers.
ocbrt_ps proc
	start:
		vpand ymm2, ymm0, [SIGN_32]
		vpxor ymm0, ymm2, ymm0
		vpxor ymm4, ymm4, ymm4
		vpsrld ymm1, ymm0, 17
		vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_32]
		vpslld ymm1, ymm1, 16
		vpaddd ymm1, ymm1, [EXP_ADDEND_32]
		vcmpps ymm3, ymm0, ymm4, 4h
		vandps ymm1, ymm1, ymm3

	newton_iterations:
		vmovaps ymm4, [ONE_THIRD_32]
		vmovaps ymm5, [TWO_THIRDS_32]
		vmulps ymm0, ymm0, ymm4
		mov ecx, 6					; change to about 33, if you have to deal with denormal numbers (is much slower though)

		it:
			vmulps ymm3, ymm1, ymm1
			vdivps ymm3, ymm0, ymm3
			vfmadd213ps ymm1, ymm5, ymm3
			dec ecx
			jnz it

		vxorps xmm0, xmm1, xmm2

	ret
ocbrt_ps endp

; Calculates the reciprocal value of the cube root of one double-precision floating-point number.
orcbrt_sd proc
	start:
		vmovq rax, xmm0
		mov r9, 8000000000000000h
		and r9, rax
		xor rax, r9
		vmovq xmm0, rax
		mov ecx, 7
		mov r8d, 35					; omit this and the following 2 instructions, if you know that no subnormal numbers occur
		test rax, [EXP_MASK_64]
		cmovz ecx, r8d
		sub rax, [EXP_MINUEND_64]
		not rax
		mul [DIV_3_64_SCALAR]

	newton_iterations:
		vmovq xmm1, rdx
		vmovsd xmm2, [ONE_THIRD_64]
		vmovsd xmm5, [FOUR_THIRDS_64]
		vmulsd xmm3, xmm0, xmm2

		it:
			vmulsd xmm4, xmm3, xmm1
			vmulsd xmm2, xmm1, xmm1
			vfnmadd213sd xmm4, xmm2, xmm5
			vmulsd xmm1, xmm1, xmm4
			dec ecx
			jnz it

		vmovq xmm2, r9
		vmovq rax, xmm0
		vmovq rcx, xmm1
		xor rax, 0
		cmovz rcx, [FP_INFINITY_64]
		vmovq xmm0, rcx
		vxorpd xmm0, xmm2, xmm1

	ret
orcbrt_sd endp

; Calculates the reciprocal value of the cube root of one single-precision floating-point number.
orcbrt_ss proc
	start:
		vmovd eax, xmm0
		mov r9d, 80000000h
		and r9d, eax
		xor eax, r9d
		vmovd xmm0, eax
		mov ecx, 4
		mov r8d, 31						; omit this and the following 2 instructions, if you know that no subnormal numbers occur
		test eax, [EXP_MASK_32]
		cmovz ecx, r8d
		sub eax, [MAGICAL_NUMBER]
		not eax
		mul [DIV_3_32_SCALAR]

	newton_iterations:
		vmovd xmm1, edx
		vmovss xmm2, [ONE_THIRD_32]
		vmovss xmm5, [FOUR_THIRDS_32]
		vmulss xmm3, xmm0, xmm2

		it:
			vmulss xmm4, xmm3, xmm1
			vmulss xmm2, xmm1, xmm1
			vfnmadd213ss xmm4, xmm2, xmm5
			vmulss xmm1, xmm1, xmm4
			dec ecx
			jnz it

		vmovd xmm2, r9d
		vmovd eax, xmm0
		vmovd ecx, xmm1
		xor eax, 0
		cmovz ecx, [FP_INFINITY_32]
		vmovd xmm1, ecx
		vxorps xmm0, xmm2, xmm1

	ret
orcbrt_ss endp

; Calculates the reciprocal value of the cube root of four double-precision floating-point numbers.
orcbrt_pd proc
	start:
		vpand ymm5, ymm0, [SIGN_64]
		vpxor ymm0, ymm0, ymm5
		vpsubq ymm1, ymm0, [EXP_MINUEND_64]
		vpxor ymm1, ymm1, [ONES_64]
		vpsrlq ymm1, ymm1, 33
		vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_64]
		vpsllq ymm1, ymm1, 32

	newton_iterations:
		vmovapd ymm2, [ONE_THIRD_64]
		vmulpd ymm3, ymm0, ymm2
		mov ecx, 7				; change to about 35, if you have to deal with denormal numbers (is much slower though)

		it:
			vmulpd ymm4, ymm3, ymm1
			vmulpd ymm2, ymm1, ymm1
			vfnmadd213pd ymm4, ymm2, [FOUR_THIRDS_64]
			vmulpd ymm1, ymm1, ymm4
			dec ecx
			jnz it

		vxorpd ymm4, ymm4, ymm4
		vcmppd ymm3, ymm0, ymm4, 4h
		vandpd ymm1, ymm1, ymm3
		vandnpd ymm2, ymm3, [FP_INFINITY_64]
		vorpd ymm1, ymm1, ymm2
		vxorpd ymm0, ymm1, ymm5

	ret
orcbrt_pd endp

; Calculates the reciprocal value of the cube root of eight single-precision floating-point numbers.
orcbrt_ps proc
	start:
		vpand ymm5, ymm0, [SIGN_32]
		vpxor ymm0, ymm0, ymm5
		vpsubd ymm1, ymm0, [MAGICAL_NUMBER]
		vpxor ymm1, ymm1, [ONES_32]
		vpsrld ymm1, ymm1, 17
		vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_32]
		vpslld ymm1, ymm1, 16

	newton_iterations:
		vmovaps ymm2, [ONE_THIRD_32]
		vmulps ymm3, ymm0, ymm2
		mov ecx, 6			; change to about 33, if you have to deal with denormal numbers (is much slower though)

		it:
			vmulps ymm4, ymm3, ymm1
			vmulps ymm2, ymm1, ymm1
			vfnmadd213ps ymm4, ymm2, [FOUR_THIRDS_32]
			vmulps ymm1, ymm1, ymm4
			dec ecx
			jnz it

		vxorps ymm4, ymm4, ymm4
		vcmpps ymm3, ymm0, ymm4, 4h
		vandps ymm1, ymm1, ymm3
		vandnps ymm2, ymm3, [FP_INFINITY_32]
		vorps ymm1, ymm1, ymm2
		vxorps ymm0, ymm1, ymm5

	ret
orcbrt_ps endp

; Adapted from the famous FISR algorithm
macro_fast_invcbrt_ss macro
	vmovd eax, xmm0
	sub eax, [MAGICAL_NUMBER]
	not eax
	mul [DIV_3_32_SCALAR]
	vmovd xmm1, edx
	vmovss xmm2, [ONE_THIRD_32]
	vmulss xmm3, xmm0, xmm2
	vmulss xmm4, xmm3, xmm1
	vmulss xmm2, xmm1, xmm1
	vfnmadd213ss xmm4, xmm2, [FOUR_THIRDS_32]
	vmulss xmm0, xmm1, xmm4
endm

; Adapted from the famous FISR algorithm
fast_invcbrt_ss proc
	macro_fast_invcbrt_ss

	ret
fast_invcbrt_ss endp

; Adapted from the famous FISR algorithm
macro_fast_invcbrt_ps macro
	vpsubd ymm1, ymm0, [MAGICAL_NUMBER]
	vpxor ymm1, ymm1, [ONES_32]
	vpsrld ymm1, ymm1, 17
	vpmulhuw ymm1, ymm1, ymmword ptr [DIV_3_32]
	vmovaps ymm2, [ONE_THIRD_32]
	vmulps ymm3, ymm0, ymm2
	vmulps ymm4, ymm3, ymm1
	vmulps ymm2, ymm1, ymm1
	vfnmadd213ps ymm4, ymm2, [FOUR_THIRDS_32]
	vmulps ymm0, ymm1, ymm4
endm

; Adapted from the famous FISR algorithm
fast_invcbrt_ps proc
	macro_fast_invcbrt_ps

	ret
fast_invcbrt_ps endp

END
