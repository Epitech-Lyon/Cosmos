/********************************************************************************
** 
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

# include <arch/x86_64/boot/boot.h>

.code32

.section .text

/*
** Bootstrap paging set up
*/
.global setup_paging
.type setup_paging, @function
setup_paging:
    /* 
    ** new-meta recursive mapping hosted in index 510 PML4
    ** Eh ?
    */
    mov $V2P(kernel_pml4), %eax
    orl $(MMU_PRESENT | MMU_WRITABLE), %eax
    mov $V2P(kernel_pml4), %edi
    movl %eax, (__RECURSIVE_MAPPING_IDX * 8)(%edi)

    /*
    ** PDP in PML4 index
    */
    mov $V2P(kernel_page_directory_pointer), %eax
    orl $(MMU_PRESENT | MMU_WRITABLE), %eax
    mov %eax, V2P(.kernel_plm4_kernel_entry)
    mov %eax, V2P(kernel_pml4)

    /*
    ** PD in PDP index
    */
    mov $V2P(kernel_page_directory), %eax
    orl $(MMU_PRESENT | MMU_WRITABLE), %eax
    mov $V2P(kernel_page_directory_pointer), %edi
    movl %eax, (KERNEL_PDP_INDEX_START * 8)(%edi)
    movl %eax, (%edi)

    /*
    ** PT in PD index
    */
    mov $V2P(kernel_page_table), %eax
    orl $(MMU_PRESENT | MMU_WRITABLE), %eax
    mov $V2P(kernel_page_directory), %edi
    movl %eax, (%edi)
    addl $(KERNEL_PD_INDEX_START * 8), %edi
    movl $40, %ecx
    .loop_pd:
        movl %eax, (%edi)
        addl $PAGE_TOTAL_SZ, %eax
        addl $0x8, %edi
        loop .loop_pd

    /*
    ** Physical address in PT index
    */
    mov $V2P(kernel_page_table), %edi
    xorl %edx, %edx
    movl $(40 * PAGE_ENTRY_NBR), %ecx
    .loop_page:
        movl %edx, %eax
        orl $(MMU_PRESENT | MMU_WRITABLE), %eax
        movl %eax, (%edi)
        addl $PAGE_TOTAL_SZ, %edx
        addl $8, %edi
        loop .loop_page

    /*
    ** load PML4 in cr3
    */
    mov $V2P(kernel_pml4), %eax
    movl %eax, %cr3

    # PAE & GP
    movl %cr4, %eax
    orl $((1 << 5) | (1 << 7)), %eax
    movl %eax, %cr4

    # LME & NXE
    movl $0xC0000080, %ecx
    rdmsr
    orl $((1 << 8) | (1 << 11)), %eax
    wrmsr

    # PE & PM & Wprotected
    movl %cr0, %eax
    orl $((1 << 31) | (1 << 16) | (1 << 0)), %eax
    movl %eax, %cr0

    # flush TLB, not really needed
    movl %cr3, %eax
    movl %eax, %cr3

    ret

/*
** Early Kernel paging data
** names are explicit
*/

.section .data

.global kernel_pml4
    .global .kernel_pml4_first_entry
    .global .kernel_plm4_kernel_entry

.align 4096
kernel_pml4:
    .rept KERNEL_PML4_INDEX_START
        .8byte 0x0
    .endr
    .kernel_plm4_kernel_entry:
        .rept (PAGE_ENTRY_NBR - KERNEL_PML4_INDEX_START)
            .8byte 0x0
        .endr

.global kernel_page_directory_pointer

.align 4096
kernel_page_directory_pointer:
    .rept PAGE_ENTRY_NBR
        .8byte 0x0
    .endr

.global kernel_page_directory

.align 4096
kernel_page_directory:
    .rept PAGE_ENTRY_NBR
        .8byte 0x0
    .endr

.align 4096
kernel_page_table:
    .rept (PAGE_ENTRY_NBR * 40)
        .8byte 0x0
    .endr
