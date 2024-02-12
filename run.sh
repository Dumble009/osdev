#!/bin/bash

set -xue

# QEMUのファイルパス
QEMU=qemu-system-riscv32

$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot