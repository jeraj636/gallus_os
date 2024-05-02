#ifndef GDT_H
#define GDT_h

#define data_segment 0b10010010
#define code_segment 0b10011010

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void segmentni_opisovalec(uint32_t base, uint32_t limit, char flags, char access_byte, uint64_t *pozicija);
void naredi_gdt();

#endif