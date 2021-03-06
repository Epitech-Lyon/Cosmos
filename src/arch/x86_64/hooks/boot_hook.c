/********************************************************************************
**
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

# include <kernel/def/def.h>
# include <kernel/init/inithooks.h>
# include <kernel/init/initcalls.h>
# include <drivers/vga.h>
# include <bios/acpi.h>
# include ARCH_HEADER(boot/multiboot2.h)
# include ARCH_HEADER(mem/vmm.h)
# include ARCH_HEADER(system/apic.h)
# include ARCH_HEADER(system/ioapic.h)

extern char const *cosmos_signature_asciiart;

/*
** Cosmos signature & acquired start
*/
static void helloFromCosmos(void)
{
    vga_set_color(VGA_BLACK, VGA_LIGHT_BLUE);
    vga_puts(cosmos_signature_asciiart);
    vga_set_color(VGA_BLACK, VGA_WHITE);
    vga_puts("\nkernel init routine...");
}

static void boot_hook(void)
{
    multiboot_parse_tags();
    run_boot_initcalls();
    helloFromCosmos();
    /*
    ** The following initializations depend of the first above
    */
    ARCH_FUNCTION_MAPPING(vmm_init)();
    acpi_init();
    apic_init();
    ioapic_init();
}

REGISTER_BOOT_INITHOOK(boot_hook);
