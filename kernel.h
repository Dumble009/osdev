#pragma once
#include "common.h"

struct sbiret
{
    int32_t error;
    int32_t value;
};

void putchar(char ch);