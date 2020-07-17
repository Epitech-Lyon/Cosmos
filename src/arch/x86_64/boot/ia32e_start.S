/********************************************************************************
** 
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

#include <arch/x86_64/boot/boot.h>

.code64

.extern bootstrap_stack_bot

.section .text

/*
** init long mode
** reaload values
** jump into kernel main
*/
.global ia32_start
.type ia32_start, @function
ia32_start:
    movw $KERNEL_DATA_SELECTOR, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    lea .higher_half, %rax
    jmp *%rax
    .higher_half:

    # reload stack
    movq $bootstrap_stack_top, %rsp

    # reload gdt
    movq $gdtptr, %rax
    lgdt (%rax)

    addq $__KERNEL_ADDR_TRNS, %rbx # mb2 address
    pushq %rbx
    // movq $multiboot_virtaddr, %rax
    movq %rbx, (%rax) # in our multiboot parser file

    jmp kernelmain