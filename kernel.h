#pragma once
#include "common.h"

#define PANIC(fmt, ...)                                                       \
    do                                                                        \
    {                                                                         \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while (1)                                                             \
        {                                                                     \
        }                                                                     \
    } while (0)

struct sbiret
{
    int32_t error;
    int32_t value;
};

void putchar(char ch);

struct trap_frame
{
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                         \
    ({                                                        \
        unsigned long __tmp;                                  \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                                \
    })

#define WRITE_CSR(reg, value)                                   \
    do                                                          \
    {                                                           \
        uint32_t __tmp = (value);                               \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
    } while (0)

#define PROCS_MAX 8     // 最大プロセス数
#define PROC_UNUSED 0   // プロセスの状態 : 未使用
#define PROC_RUNNABLE 1 // プロセスの状態 : 実行可能
#define PROC_EXITED 2   // プロセスの状態 : 終了済み

/// @brief PCBの実体となる構造体
struct process
{
    int pid;              // プロセスID
    int state;            // プロセスの状態
    vaddr_t sp;           // コンテキストスイッチ時のカーネルスタックポインタ
    uint32_t *page_table; // プロセスのページテーブル
    uint8_t stack[8192];  // カーネルスタック
};

// ページング機構の定数
#define SATP_SV32 (1u << 31) // Sv32有効化ビット
#define PAGE_V (1 << 0)      // 有効かどうか
#define PAGE_R (1 << 1)      // 読み込み可能か
#define PAGE_W (1 << 2)      // 書き込み可能か
#define PAGE_X (1 << 3)      // 実行可能か
#define PAGE_U (1 << 4)      // ユーザーモードでアクセス可能か

// ユーザープログラムのロード先
#define USER_BASE 0x1000000

// ユーザーモードへの切り替え時に割り込みを有効化するフラグ
#define SSTATUS_SPIE (1 << 5)

#define SCAUSE_ECALL 8 // 例外の発生要因 : システムコール