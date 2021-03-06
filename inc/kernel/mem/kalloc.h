/*******************************************************************************
**
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

#ifndef _KERNEL_MEM_KALLOC_H_
# define _KERNEL_MEM_KALLOC_H_

# include <cosmos.h>
# include <kernel/mem/memory.h>
# include <kernel/mem/vmm.h>

virtaddr_t kalloc(size_t size);
virtaddr_t kalloc_aligned(size_t size, size_t alignement);
virtaddr_t krealloc(virtaddr_t virt, size_t size);
virtaddr_t kalloc_dev(physaddr_t phys, size_t size);
virtaddr_t kmap_dev(physaddr_t phys, size_t size);
void kalloc_init(void);
void kalloc_dump(void);
void kfree(virtaddr_t virt);

#endif /* !_KERNEL_MEM_KALLOC_H_ */
