#include "kernel.h"
#include "common.h"

extern char __bss[], __bss_end[], __stack_top[];

/// @brief 指定されたメモリ領域を指定された値で埋める
/// @param buf 値埋めするメモリ領域の先頭アドレス
/// @param c 埋めたい値
/// @param n bufから何バイト分を値埋めするか
/// @return bufと同じ値
void *memset(void *buf, char c, size_t n)
{
    uint8_t *p = (uint8_t *)buf;
    while (n--)
    {
        *p = c;
        *p += 1;
    }
    return buf;
}

/// @brief SBIの処理を呼び出す関数
/// @param arg0~arg5 SBIの処理に渡す引数
/// @param fid ecall命令に渡すSBIのfunction id
/// @param eid ecall命令に渡す拡張セットのID
struct sbiret sbi_call(
    int64_t arg0,
    int64_t arg1,
    int64_t arg2,
    int64_t arg3,
    int64_t arg4,
    int64_t arg5,
    int64_t fid,
    int64_t eid)
{
    register int64_t a0 __asm__("a0") = arg0;
    register int64_t a1 __asm__("a1") = arg1;
    register int64_t a2 __asm__("a2") = arg2;
    register int64_t a3 __asm__("a3") = arg3;
    register int64_t a4 __asm__("a4") = arg4;
    register int64_t a5 __asm__("a5") = arg5;
    register int64_t a6 __asm__("a6") = fid;
    register int64_t a7 __asm__("a7") = eid;

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