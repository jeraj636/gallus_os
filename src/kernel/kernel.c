#include <stdint.h>
#include "preprost_vga_gonilnik/preprost_vga_gonilnik.h"

int kmain()
{
	vga_inicializiraj_terminal();
	vga_izpisi_niz("Pozdravljen svet!\nTo je Gallus OS!");
	while(1);
}


