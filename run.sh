#!/bin/bash

set -xue

# QEMUのファイルパス
QEMU=qemu-system-riscv32

CC=clang

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# カーネルのビルドコマンド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf kernel.c

# QEMUの実行
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf