#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef long int32_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint32_t size_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

#define true 1
#define false 0
#define NULL ((void *)0)
#define align_up(value, align) __builtin_align_up(value, align)
#define is_aligned(value, align) __builtin_is_aligned(value, align)
#define offsetof(value, member) __builtin_offsetof(type, member)

#define PAGE_SIZE 4096

#define SYS_PUTCHAR 1 // putcharのシステムコール識別番号

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

/// @brief 指定されたメモリ領域を指定された値で埋める
/// @param buf 値埋めするメモリ領域の先頭アドレス
/// @param c 埋めたい値
/// @param n bufから何バイト分を値埋めするか
/// @return bufと同じ値
void *memset(void *buf, char c, size_t n);

/// @brief あるメモリ領域の値を別のメモリ領域にコピーする
/// @param dst コピー先のメモリ領域のアドレス
/// @param src コピー元のメモリ領域のアドレス
/// @param n コピーしたいバイト数
/// @return dstと同じアドレス
void *memcpy(void *dst, const void *src, size_t n);

/// @brief 文字列をコピーする。ヌル文字を見つけるまでコピーを行う
/// @param dst コピー先の文字列の先頭アドレス
/// @param src コピー元の文字列の先頭アドレス
/// @return dstと同じアドレス
char *strcpy(char *dst, const char *src);

/// @brief 文字列が一致するか比較する。ヌル文字を見つけるまで比較する
/// @param s1 比較対象の文字列1
/// @param s2 比較対象の文字列2
/// @return s1の方が辞書順で先ならば負、s2の方が辞書順で後ならば正の値。一致すれば0
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);