#pragma once
#include <cstdint>

extern  "C" {
uintptr_t MainHandle = 0;
uintptr_t CopyPasteHook = 0x650BF2;
uintptr_t JumpBackFromCopyPasteHook = 0;

uintptr_t CMInstance = 0;
uintptr_t KeyState = 0;

uintptr_t s_rax = 0;
uintptr_t s_rbx = 0;
uintptr_t s_rcx = 0;
uintptr_t s_rdx = 0;
uintptr_t s_rsi = 0;
uintptr_t s_rdi = 0;
uintptr_t s_rbp = 0;
uintptr_t s_rsp = 0;
uintptr_t s_r8 = 0;
uintptr_t s_r9 = 0;
uintptr_t s_r10 = 0;
uintptr_t s_r11 = 0;
uintptr_t s_r12 = 0;
uintptr_t s_r13 = 0;
uintptr_t s_r14 = 0;
uintptr_t s_r15 = 0;
}