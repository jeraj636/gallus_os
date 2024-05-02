#include <stdint.h>
#include "preprost_vga_gonilnik/preprost_vga_gonilnik.h"
#include "c_lib/stdio.h"
#include "gdt/gdt.h"

int kmain()
{
	vga_inicializiraj_terminal();
	printf("Pozdravljen svet!\nTo je gallus_os!\n");
	naredi_gdt();
	printf("Zdaj naj bi deloval GDT!\n");

	while (1)
		;
}
