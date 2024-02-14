#include "test.h"
#include "common.h"
#include "kernel.h"

void memset_test()
{
    char cs[] = "abcdefgh";
    memset(cs, '0', 4);
    for (int i = 0; i < 4; i++)
    {
        if (cs[i] != '0')
        {
            PANIC("memset_test failed! : memset doesn't set values properly.");
        }
    }

    if (cs[4] != 'e' || cs[5] != 'f' || cs[6] != 'g' || cs[7] != 'h')
    {
        PANIC("memset_test failed! : memset sets values on invalid area.");
    }

    printf("memset_test passed!\n");
}

void memcpy_test()
{
    char cs1[] = "abcdefgh";
    char cs2[] = "12345678";
    char org_cs2[] = "12345678";
    memcpy(cs1, cs2, 4);
    for (int i = 0; i < 4; i++)
    {
        if (cs1[i] != cs2[i])
        {
            PANIC("memcpy_test failed! : memcpy doesn't copy values properly.");
        }
    }

    if (cs1[4] != 'e' || cs1[5] != 'f' || cs1[6] != 'g' || cs1[7] != 'h')
    {
        PANIC("memcpy_test failed! : memcpy copies values on invalid area.");
    }

    for (int i = 0; i < 8; i++)
    {
        if (cs2[i] != org_cs2[i])
        {
            PANIC("memcpy_test failed! : memcpy modifies src.");
        }
    }

    printf("memcpy_test passed!\n");
}

void strcpy_test()
{
    char cs1[] = "abcdefgh";
    char cs2[] = "12345678";
    char org_cs2[] = "12345678";
    strcpy(cs1, cs2);
    for (int i = 0; i < 8; i++)
    {
        if (cs1[i] != cs2[i])
        {
            PANIC("strcpy_test failed! : strcpy doesn't copy values properly.");
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (cs2[i] != org_cs2[i])
        {
            PANIC("strcpy_test failed! : strcpy modifies src.");
        }
    }

    char cs3[] = "abcd";
    strcpy(cs1, cs3);
    for (int i = 0; i < 4; i++)
    {
        if (cs1[i] != cs3[i])
        {
            PANIC("strcpy_test failed! : strcpy doesn't copy values properly, when src is shorter than dst.");
        }
    }
    if (cs1[4] != '\0')
    {
        PANIC("strcpy_test failed! : strcpy doesn't put a null character properly, when src is shorter than dst.");
    }

    printf("strcpy_test passed!\n");
    // srcがdstより長い時はそもそもコピーが上手くいかないので、テストはしない。
}

void strcmp_test()
{
    char cs1[] = "abc";
    char cs2[] = "abc";
    if (strcmp(cs1, cs2))
    {
        PANIC("strcmp_test failed! : strcmp can't judge properly, when s1 and s2 are identical.");
    }

    char cs3[] = "aac";
    if (strcmp(cs1, cs3) <= 0)
    {
        PANIC("strcmp_test failed! : strcmp can't judge properly when s2 is smaller than s1.");
    }

    char cs4[] = "acc";
    if (strcmp(cs1, cs4) >= 0)
    {
        PANIC("strcmp_test failed! : strcmp can't judge properly when s2 is greater than s1.");
    }

    char cs5[] = "abcdef";
    if (strcmp(cs1, cs5) >= 0)
    {
        PANIC("strcmp_test failed! : strcmp can't judge properly when s2 is greater and longer than s1.");
    }

    char cs6[] = "aacdef";
    if (strcmp(cs1, cs6) <= 0)
    {
        PANIC("strcmp_test failed! : strcmp can't judge properly when s2 is smaller and longer than s1.");
    }

    printf("strcmp_test passed!\n");
}

void run_test()
{
    memset_test();
    memcpy_test();
    strcpy_test();
    strcmp_test();
}