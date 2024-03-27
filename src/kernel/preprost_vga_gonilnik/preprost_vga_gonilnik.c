#include "preprost_vga_gonilnik.h"

static const size_t VGA_VISINA = 25;
static const size_t VGA_DOLZINA = 80;

size_t terminal_vrstica;
size_t terminal_stolpec;
uint8_t terminal_barvna_kombinacija;
uint16_t *terminal_medpomnilnik;

void vga_inicializiraj_terminal(void)
{
	terminal_vrstica = 0;
	terminal_stolpec = 0;
	terminal_barvna_kombinacija = vga_barvna_kombinacija(VGA_BARVA_BELA, VGA_BARVA_CRNA);
	terminal_medpomnilnik = (uint16_t*)0xb8000;

	for(int i = 0; i < VGA_VISINA; i++)
		for(int j = 0; j < VGA_DOLZINA; j++)
			terminal_medpomnilnik[i * VGA_DOLZINA + j] = vga_barvni_znak(terminal_barvna_kombinacija, ' ');

}

void nastavi_barvno_kombinacijo_terminala(uint8_t barvna_kombinacija)
{
	terminal_barvna_kombinacija  = barvna_kombinacija;
}

void vga_izpisi_barvni_znak(char c, uint8_t barvna_kombinacija, size_t x, size_t y)
{
	terminal_medpomnilnik[y * VGA_DOLZINA + x] = vga_barvni_znak(barvna_kombinacija, c);
}

void vga_izpisi_znak(char c)
{
	vga_izpisi_barvni_znak(c, terminal_barvna_kombinacija, terminal_vrstica, terminal_stolpec);
	if(++terminal_stolpec == VGA_DOLZINA)
	{
		terminal_stolpec = 0;
		if(++terminal_vrstica == VGA_VISINA)
			terminal_vrstica = 0;
	}
}

void vga_izpisi_niz(const char * niz)
{
	for(int i = 0; niz[i] != '\0'; i++)
	{
		if(niz[i] == '\n')
		{
			terminal_stolpec = 0;
			if(++terminal_vrstica == VGA_VISINA)
				terminal_vrstica = 0;
			continue;
		}


		vga_izpisi_znak(niz[i]);
	}
}
