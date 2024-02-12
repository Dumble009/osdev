#pragma once

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;
typedef long int64_t;

struct sbiret
{
    int64_t error;
    int64_t value;
};

void putchar(char ch);