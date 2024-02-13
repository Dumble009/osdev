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
            PANIC("memset_test failed! : memset doesn't set value properly.");
        }
    }

    if (cs[4] != 'e' || cs[5] != 'f' || cs[6] != 'g' || cs[7] != 'h')
    {
        PANIC("memset_test failed! : memset sets values on invalid area.");
    }

    printf("memset_test passed!\n");
}

void run_test()
{
    memset_test();
}