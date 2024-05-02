#include "gdt.h"
#include "../c_lib/stdio.h"
#include "../c_lib/string.h"

void segmentni_opisovalec(uint32_t base, uint32_t limit, char flags, char access_byte, uint64_t *pozicija)
{
    char *poz = (char *)pozicija;
    memcpy(poz, &base, 1);
    flags <<= 4;
    flags &= 0xf0;
    flags |= (*(char *)(&limit)) & 0xf0;
    memcpy(poz + 1, &flags, 1);
    memcpy(poz + 2, &access_byte, 1);
    memcpy(poz + 3, ((char *)(&base) + 1), 3);
    memcpy(poz + 6, ((char *)(&limit) + 2), 2);
}
struct gdt_opisovalec
{
    uint16_t vel;
    uint32_t poz;
} __attribute__((packed));

void naredi_gdt()
{
    segmentni_opisovalec(0, 0, 0, 0, (uint64_t *)0);
    segmentni_opisovalec(0, 0x000fffff, 0x4, data_segment, (uint64_t *)64);
    segmentni_opisovalec(0x00f00000, 0x000fffff, 0x4, code_segment, (uint64_t *)128);

    struct gdt_opisovalec gdt;
    gdt.poz = 0;
    gdt.vel = 127;

    asm volatile("lgdt (%0)" : : "r"(&gdt));
}