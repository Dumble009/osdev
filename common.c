#include "common.h"
#include "kernel.h"

#define special_character '%'
#define string_mark 's'
#define dec_mark 'd'
#define hex_mark 'x'

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
        *p++ = c;
    }
    return buf;
}

/// @brief あるメモリ領域の値を別のメモリ領域にコピーする
/// @param dst コピー先のメモリ領域のアドレス
/// @param src コピー元のメモリ領域のアドレス
/// @param n コピーしたいバイト数
/// @return dstと同じアドレス
void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *p = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (n--)
    {
        *p++ = *s++;
    }

    return p;
}

/// @brief 文字列をコピーする。ヌル文字を見つけるまでコピーを行う
/// @param dst コピー先の文字列の先頭アドレス
/// @param src コピー元の文字列の先頭アドレス
/// @return dstと同じアドレス
char *strcpy(char *dst, const char *src)
{
    char *d = dst;
    while (*src)
    {
        *d++ = *src++;
    }
    // *srcがヌル文字列になったところでコピーが止まるので、
    // 最後にヌル文字列をdstにコピーしてやる必要がある。
    *d = '\0';
    return dst;
}

/// @brief 文字列が一致するか比較する。ヌル文字を見つけるまで比較する
/// @param s1 比較対象の文字列1
/// @param s2 比較対象の文字列2
/// @return s1の方が辞書順で先ならば負、s2の方が辞書順で後ならば正の値。一致すれば0
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0')
        {
            break;
        }
        s1 += 1;
        s2 += 1;
    }

    // 文字の大小を比較する際はunsignedにするとPOSIXで定められているので、それに従う
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void printf(const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt)
    {
        if (*fmt == special_character)
        {
            fmt++;
            switch (*fmt)
            {
            case '\0':
                putchar(special_character);
                goto end;

            case special_character:
                putchar(special_character);
                break;

            case 's':
            {
                const char *s = va_arg(vargs, const char *);
                while (*s)
                {
                    putchar(*s);
                    s++;
                }
                break;
            }

            // 10進数を出力
            case dec_mark:
            {
                int value = va_arg(vargs, int);
                // 負の数の場合は先にマイナスを出力してから絶対値を出力
                if (value < 0)
                {
                    putchar('-');
                    value *= -1;
                }

                // 上の桁から出力していく必要があるので、
                // 最大桁が1となる10の倍数divisorを作成
                int divisor = 1;
                while (value / divisor > 9)
                {
                    divisor *= 10;
                }

                while (divisor > 0)
                {
                    putchar('0' + value / divisor);
                    value %= divisor;
                    divisor /= 10;
                }

                break;
            }

            // 16進数を出力
            case hex_mark:
            {
                int value = va_arg(vargs, int);
                // 上から4ビットずつを値としてみて、それに対応する文字を出力
                for (int i = 7; i >= 0; i--)
                {
                    int nibble = (value >> (i * 4)) & 0xf;
                    putchar("0123456789abcdef"[nibble]);
                }
            }
            }
        }
        else
        {
            putchar(*fmt);
        }
        fmt += 1;
    }

end:
    va_end(vargs);
}