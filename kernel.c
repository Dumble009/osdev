#include "kernel.h"
#include "common.h"
#include "test.h"

extern char __bss[], __bss_end[], __stack_top[];

/// @brief SBIの処理を呼び出す関数
/// @param arg0~arg5 SBIの処理に渡す引数
/// @param fid ecall命令に渡すSBIのfunction id
/// @param eid ecall命令に渡す拡張セットのID
struct sbiret sbi_call(
    int32_t arg0,
    int32_t arg1,
    int32_t arg2,
    int32_t arg3,
    int32_t arg4,
    int32_t arg5,
    int32_t fid,
    int32_t eid)
{
    register int32_t a0 __asm__("a0") = arg0;
    register int32_t a1 __asm__("a1") = arg1;
    register int32_t a2 __asm__("a2") = arg2;
    register int32_t a3 __asm__("a3") = arg3;
    register int32_t a4 __asm__("a4") = arg4;
    register int32_t a5 __asm__("a5") = arg5;
    register int32_t a6 __asm__("a6") = fid;
    register int32_t a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4),
                           "r"(a5), "r"(a6), "r"(a7)
                         : "memory");
    return (struct sbiret){.error = a0, .value = a1};
}

/// @brief 指定した文字を標準出力に表示する
/// @param ch 表示したい文字
void putchar(char ch)
{
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

void kernel_main(void)
{
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    printf("\n\nHellow %s\n", "World!");
    printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);

    run_test();

    // 無限ループ
    for (;;)
    {
        __asm__ __volatile__("wfi");
    }
}

__attribute__((section(".text.boot")))
__attribute__((naked)) void
boot(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "j kernel_main\n"
        :
        : [stack_top] "r"(__stack_top));
}