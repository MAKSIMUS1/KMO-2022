.586
.model flat, stdcall
includelib StandartLib.lib
includelib kernel32.lib
includelib libucrt.lib

ExitProcess PROTO : DWORD
copystr PROTO : DWORD, : DWORD
concatstr PROTO : DWORD, : DWORD
outstr PROTO : DWORD
outint PROTO : SDWORD 
.stack 4096
.const
	overflow db 'ERROR: VARIABLE OVERFLOW', 0 
	null_division db 'ERROR: DIVISION BY ZERO', 0
	L1 SDWORD 5
	L2 SDWORD 6
	L3 SDWORD 25
	L4 SDWORD 2
	L5 BYTE "nachalo", 0
	L6 SDWORD 10
	L7 BYTE "konez", 0
	L8 BYTE "123456", 0
	L9 BYTE "7890", 0
	L10 SDWORD 27
	L11 SDWORD 4
	L12 SDWORD -5
	L13 BYTE "hello", 0
	L14 BYTE "tu rbo", 0
	L15 SDWORD -61
	L16 SDWORD 26
.data
	puj_z SDWORD 0
	fs_c3 BYTE 255 DUP(0)
	main_i SDWORD 0
	main_g SDWORD 0
	main_sa BYTE 255 DUP(0)
	main_sb BYTE 255 DUP(0)
	main_sc BYTE 255 DUP(0)
	main_tre SDWORD 0
	main_sbb BYTE 255 DUP(0)
	main_scc BYTE 255 DUP(0)
	main_buf BYTE 255 DUP(0)
.code

puj_proc PROC, puj_ok : SDWORD
	push L1
	push L2
	pop eax
	pop ebx
	imul eax, ebx
	jo EXIT_OVERFLOW
	push eax
	push puj_ok
	pop eax
	neg eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop puj_z
	push puj_z

	jmp EXIT
	EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

	EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:
	pop eax
	ret 4

	push 0
	call ExitProcess

puj_proc ENDP

fs_proc PROC, fs_a : DWORD, fs_b : DWORD
	push fs_a
	push fs_b
	call concatstr
	push eax
	push offset fs_c3
	call copystr

	push offset fs_c3

	jmp EXIT
	EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

	EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:
	pop eax
	ret 8

	push 0
	call ExitProcess

fs_proc ENDP

main PROC
	push L3
	pop main_i
	push L4
	pop main_g
	push offset L5
	call outstr

	CYCLE1: 
	push main_i
	call outint

	push main_i
	push main_g
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop main_i
	cmp eax,L6 
	jge NEXT1
	loop CYCLE1
	NEXT1:
	push offset L7
	call outstr

	push offset L8
	push offset main_sa
	call copystr

	push offset L9
	push offset main_sb
	call copystr

	push offset main_sb
	push offset main_sa
	call fs_proc
	push eax
	push offset main_sc
	call copystr

	push offset main_sc
	call outstr

	push L10
	push L11
	call puj_proc
	push eax
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	push L12
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop main_tre
	push main_tre
	call outint

	push offset L13
	push offset main_sbb
	call copystr

	push offset L14
	push offset main_scc
	call copystr

	push offset main_scc
	push offset main_sbb
	call copystr
	push eax
	push offset main_buf
	call copystr

	push offset main_buf
	call outstr

	push L15
	call outint

	push L16
	call outint


	jmp EXIT
	EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

	EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:
	push 0
	call ExitProcess

main ENDP
end main