#ifndef MEMMAP_H
#define MEMMAP_H
#include <efi.h>
#include <efilib.h>

extern const CHAR16 *MemoryTypeNames[];

typedef struct
{
    EFI_MEMORY_DESCRIPTOR *efi_memory_map;
    UINTN efi_memory_map_size;
    UINTN efi_descriptor_size;
} Memmap;

void get_memmap(Memmap *map);

void free_memmap(Memmap *map);
#endif
