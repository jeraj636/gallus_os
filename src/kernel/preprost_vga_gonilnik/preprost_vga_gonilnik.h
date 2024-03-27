#ifndef PREPROST_VGA_GONILNIK
#define PREPROST_VGA_GONILNIK

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
enum vga_barva
{
	VGA_BARVA_CRNA = 0,
	VGA_BARVA_MODRA = 1,
	VGA_BARVA_ZELENA = 2,
	VGA_BARVA_CIAN = 3,
	VGA_BARVA_RDECA = 4,
	VGA_BARVA_MAGENTA = 5,
	VGA_BARVA_RJAVA  = 6,
	VGA_BARVA_SVETLO_SIVA = 7,
	VGA_BARVA_TEMNO_SIVA = 8,
	VGA_BARVA_SVETLO_MODRA = 9,
	VGA_BARVA_SVETLO_ZELENA = 10,
	VGA_BARVA_SVETLO_CIAN = 11,
	VGA_BARVA_SVETLO_RDECA = 12,
	VGA_BARVA_SVETLO_MAGENTA = 13,
	VGA_BARVA_SVETLO_RJAVA = 14,
	VGA_BARVA_BELA  = 15
};



static inline uint8_t vga_barvna_kombinacija(enum vga_barva spredaj, enum vga_barva zadaj)
{
	return spredaj | zadaj << 4;
}
static inline uint16_t vga_barvni_znak(uint8_t barvna_kombinacija, char c)
{
	return (uint16_t) c | (uint16_t) barvna_kombinacija << 8; 
}
void vga_inicializiraj_terminal(void);
void vga_nastavi_barvno_kombinacijo_terminala(uint8_t barvna_kombinacija);
void vga_izpisi_barvni_znak(char c, uint8_t barvna_kombinacija, size_t x, size_t y);
void vga_izpisi_znak(char c);
void vga_izpisi_niz(const char * niz);

#endif
