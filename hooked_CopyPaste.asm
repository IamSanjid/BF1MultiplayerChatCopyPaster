EXTERNDEF JumpBackFromCopyPasteHook: QWORD
EXTERNDEF CMInstance: QWORD
EXTERNDEF KeyState: QWORD

EXTERNDEF s_rax: QWORD
EXTERNDEF s_rbx: QWORD
EXTERNDEF s_rcx: QWORD
EXTERNDEF s_rdx: QWORD
EXTERNDEF s_rsi: QWORD
EXTERNDEF s_rdi: QWORD
EXTERNDEF s_rbp: QWORD
EXTERNDEF s_rsp: QWORD
EXTERNDEF s_r8 : QWORD
EXTERNDEF s_r9 : QWORD
EXTERNDEF s_r10: QWORD
EXTERNDEF s_r11: QWORD
EXTERNDEF s_r12: QWORD
EXTERNDEF s_r13: QWORD
EXTERNDEF s_r14: QWORD
EXTERNDEF s_r15: QWORD

PerformCopyPaste PROTO C

.code
hooked_CopyPaste proc
	mov [s_rax], rax
	mov [s_rbx], rbx
	mov [s_rcx], rcx
	mov [s_rdx], rdx
	mov [s_rsi], rsi
	mov [s_rdi], rdi
	mov [s_rbp], rbp
	mov [s_rsp], rsp
	mov [s_r8 ], r8
	mov [s_r9 ], r9
	mov [s_r10], r10
	mov [s_r11], r11
	mov [s_r12], r12
	mov [s_r13], r13
	mov [s_r14], r14
	mov [s_r15], r15

	mov [CMInstance], rdi
    mov [KeyState], rax

	call PerformCopyPaste

	mov rax, [s_rax]
	mov rbx, [s_rbx]
	mov rcx, [s_rcx]
	mov rdx, [s_rdx]
	mov rsi, [s_rsi]
	mov rdi, [s_rdi]
	mov rbp, [s_rbp]
	mov rsp, [s_rsp]
	mov r8 , [s_r8 ]
	mov r9 , [s_r9 ]
	mov r10, [s_r10]
	mov r11, [s_r11]
	mov r12, [s_r12]
	mov r13, [s_r13]
	mov r14, [s_r14]
	mov r15, [s_r15]

	mov rcx, [rsp + 60h]

	jmp [JumpBackFromCopyPasteHook]
hooked_CopyPaste endp
end