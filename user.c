#include "user.h"

/// @brief システムコールの呼び出し
/// @param sysno 呼び出したいシステムコールの識別番号
/// @param arg0 システムコールに渡す引数
/// @param arg1 システムコールに渡す引数
/// @param arg2 システムコールに渡す引数
/// @return システムコールの実行結果
int syscall(int sysno, int arg0, int arg1, int arg2)
{
    register int a0 __asm__("a0") = arg0;
    register int a1 __asm__("a1") = arg1;
    register int a2 __asm__("a2") = arg2;
    register int a3 __asm__("a3") = sysno;

    __asm__ __volatile__("ecall"
                         : "=r"(a0)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3)
                         : "memory");
}

extern char __stack_top[];

__attribute__((noreturn)) void exit(void)
{
    for (;;)
        ;
}

void putchar(char ch)
{
    syscall(SYS_PUTCHAR, ch, 0, 0);
}

int getchar(void)
{
    return syscall(SYS_GETCHAR, 0, 0, 0);
}

__attribute__((section(".text.start")))
__attribute__((naked)) void
start(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "call main\n"
        "call exit\n" ::[stack_top] "r"(__stack_top));
}