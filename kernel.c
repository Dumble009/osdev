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

__attribute__((naked))
__attribute__((aligned(4))) void
kernel_entry(void)
{
    __asm__ __volatile__(
        "csrw sscratch, sp\n"
        "addi sp, sp, -4 * 31\n"
        "sw ra,  4 * 0(sp)\n"
        "sw gp,  4 * 1(sp)\n"
        "sw tp,  4 * 2(sp)\n"
        "sw t0,  4 * 3(sp)\n"
        "sw t1,  4 * 4(sp)\n"
        "sw t2,  4 * 5(sp)\n"
        "sw t3,  4 * 6(sp)\n"
        "sw t4,  4 * 7(sp)\n"
        "sw t5,  4 * 8(sp)\n"
        "sw t6,  4 * 9(sp)\n"
        "sw a0,  4 * 10(sp)\n"
        "sw a1,  4 * 11(sp)\n"
        "sw a2,  4 * 12(sp)\n"
        "sw a3,  4 * 13(sp)\n"
        "sw a4,  4 * 14(sp)\n"
        "sw a5,  4 * 15(sp)\n"
        "sw a6,  4 * 16(sp)\n"
        "sw a7,  4 * 17(sp)\n"
        "sw s0,  4 * 18(sp)\n"
        "sw s1,  4 * 19(sp)\n"
        "sw s2,  4 * 20(sp)\n"
        "sw s3,  4 * 21(sp)\n"
        "sw s4,  4 * 22(sp)\n"
        "sw s5,  4 * 23(sp)\n"
        "sw s6,  4 * 24(sp)\n"
        "sw s7,  4 * 25(sp)\n"
        "sw s8,  4 * 26(sp)\n"
        "sw s9,  4 * 27(sp)\n"
        "sw s10, 4 * 28(sp)\n"
        "sw s11, 4 * 29(sp)\n"

        "csrr a0, sscratch\n"
        "sw a0, 4 * 30(sp)\n"

        "mv a0, sp\n"
        "call handle_trap\n"

        "lw ra,  4 * 0(sp)\n"
        "lw gp,  4 * 1(sp)\n"
        "lw tp,  4 * 2(sp)\n"
        "lw t0,  4 * 3(sp)\n"
        "lw t1,  4 * 4(sp)\n"
        "lw t2,  4 * 5(sp)\n"
        "lw t3,  4 * 6(sp)\n"
        "lw t4,  4 * 7(sp)\n"
        "lw t5,  4 * 8(sp)\n"
        "lw t6,  4 * 9(sp)\n"
        "lw a0,  4 * 10(sp)\n"
        "lw a1,  4 * 11(sp)\n"
        "lw a2,  4 * 12(sp)\n"
        "lw a3,  4 * 13(sp)\n"
        "lw a4,  4 * 14(sp)\n"
        "lw a5,  4 * 15(sp)\n"
        "lw a6,  4 * 16(sp)\n"
        "lw a7,  4 * 17(sp)\n"
        "lw s0,  4 * 18(sp)\n"
        "lw s1,  4 * 19(sp)\n"
        "lw s2,  4 * 20(sp)\n"
        "lw s3,  4 * 21(sp)\n"
        "lw s4,  4 * 22(sp)\n"
        "lw s5,  4 * 23(sp)\n"
        "lw s6,  4 * 24(sp)\n"
        "lw s7,  4 * 25(sp)\n"
        "lw s8,  4 * 26(sp)\n"
        "lw s9,  4 * 27(sp)\n"
        "lw s10, 4 * 28(sp)\n"
        "lw s11, 4 * 29(sp)\n"
        "lw sp,  4 * 30(sp)\n"
        "sret\n");
}

void handle_trap(struct trap_frame *f)
{
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);

    PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n",
          scause,
          stval,
          user_pc);
}

extern char __free_ram[], __free_ram_end[];

/// @brief 指定されたページサイズのメモリ領域を確保する。
/// @param n 何ページ分の領域を確保したいのか
/// @return 確保されたメモリ領域の先頭アドレス
paddr_t alloc_pages(uint32_t n)
{
    static paddr_t next_paddr = (paddr_t)__free_ram;
    paddr_t paddr = next_paddr;
    next_paddr += n * PAGE_SIZE;

    if (next_paddr > (paddr_t)__free_ram_end)
    {
        PANIC("out of memory");
    }

    memset((void *)paddr, 0, n * PAGE_SIZE);
    return paddr;
}

struct process procs[PROCS_MAX];

struct process *create_process(uint32_t pc)
{
    // 空いているプロセス管理構造体の探索
    struct process *proc = NULL;
    int i = 0;
    for (; i < PROCS_MAX; i++)
    {
        if (procs[i].state == PROC_UNUSED)
        {
            proc = &procs[i];
            break;
        }
    }

    if (!proc)
    {
        PANIC("no free process slots");
    }

    // switch_context()を呼び出されても大丈夫なように、スタックにレジスタの値を積む
    // スタックは下に伸びていくので、stackの最大番地がspの初期値になる。
    uint32_t *sp = (uint32_t *)&proc->stack[sizeof(proc->stack)];
    // s11~s0までの初期値を積む
    for (int j = 0; j < 12; j++)
    {
        *--sp = 0;
    }
    *--sp = (uint32_t)pc; // ra(return address)の初期化

    // 各フィールドの初期化
    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->sp = (uint32_t)sp;
    return proc;
}

__attribute__((naked)) void switch_context(
    uint32_t *prev_sp,
    uint32_t *next_sp)
{
    __asm__ __volatile__(
        "addi sp, sp, -13 * 4\n"
        "sw ra,  0  * 4(sp)\n"
        "sw s0,  1  * 4(sp)\n"
        "sw s1,  2  * 4(sp)\n"
        "sw s2,  3  * 4(sp)\n"
        "sw s3,  4  * 4(sp)\n"
        "sw s4,  5  * 4(sp)\n"
        "sw s5,  6  * 4(sp)\n"
        "sw s6,  7  * 4(sp)\n"
        "sw s7,  8  * 4(sp)\n"
        "sw s8,  9  * 4(sp)\n"
        "sw s9,  10 * 4(sp)\n"
        "sw s10, 11 * 4(sp)\n"
        "sw s11, 12 * 4(sp)\n"
        "sw sp, (a0)\n"
        "lw sp, (a1)\n"
        "lw ra,  0  * 4(sp)\n"
        "lw s0,  1  * 4(sp)\n"
        "lw s1,  2  * 4(sp)\n"
        "lw s2,  3  * 4(sp)\n"
        "lw s3,  4  * 4(sp)\n"
        "lw s4,  5  * 4(sp)\n"
        "lw s5,  6  * 4(sp)\n"
        "lw s6,  7  * 4(sp)\n"
        "lw s7,  8  * 4(sp)\n"
        "lw s8,  9  * 4(sp)\n"
        "lw s9,  10 * 4(sp)\n"
        "lw s10, 11 * 4(sp)\n"
        "lw s11, 12 * 4(sp)\n"
        "addi sp, sp, 13 * 4\n"
        "ret\n");
}

struct process *current_proc; // 現在実行中のプロセス
struct process *idle_proc;    // アイドルプロセス

void yield(void)
{
    // 次に実行されるプロセスの探索
    struct process *next = idle_proc;
    for (int i = 0; i < PROCS_MAX; i++)
    {
        struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
        if (proc->state == PROC_RUNNABLE && proc->pid > 0)
        {
            next = proc;
            break;
        }
    }

    if (next == current_proc)
    {
        return;
    }

    struct process *prev = current_proc;
    current_proc = next;
    switch_context(&prev->sp, &next->sp);
}

// プロセス管理の動作テスト ----------
struct process *proc_a;
struct process *proc_b;

void proc_a_entry(void)
{
    printf("starting process A\n");
    while (1)
    {
        putchar('A');
        yield();

        for (int i = 0; i < 30000000; i++)
        {
            __asm__ __volatile__("nop");
        }
    }
}

void proc_b_entry(void)
{
    printf("starting process B\n");
    while (1)
    {
        putchar('B');
        yield();

        for (int i = 0; i < 30000000; i++)
        {
            __asm__ __volatile__("nop");
        }
    }
}
// プロセス管理の動作テスト ----------

void kernel_main(void)
{
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    // 例外ハンドラのアドレスをレジスタに登録
    WRITE_CSR(stvec, (uint32_t)kernel_entry);

    idle_proc = create_process((uint32_t)NULL);
    idle_proc->pid = -1;
    current_proc = idle_proc;

    proc_b = create_process((uint32_t)proc_b_entry);
    proc_a = create_process((uint32_t)proc_a_entry);
    yield();
    PANIC("switched to idle process");

    // 標準ライブラリのテスト
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