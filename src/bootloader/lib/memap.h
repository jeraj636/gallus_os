#ifndef MEMMAP_H
#define MEMMAP_H
#include <efi.h>
#include <efilib.h>

const CHAR16 *MemoryTypeNames[] = {
    L"EfiReservedMemoryType",
    L"EfiLoaderCode",
    L"EfiLoaderData",
    L"EfiBootServicesCode",
    L"EfiBootServicesData",
    L"EfiRuntimeServicesCode",
    L"EfiRuntimeServicesData",
    L"EfiConventionalMemory",
    L"EfiUnusableMemory",
    L"EfiACPIReclaimMemory",
    L"EfiACPIMemoryNVS",
    L"EfiMemoryMappedIO",
    L"EfiMemoryMappedIOPortSpace",
    L"EfiPalCode",
    L"EfiMaxMemoryType"};

typedef struct
{
    EFI_MEMORY_DESCRIPTOR *efi_memory_map;
    UINTN efi_memory_map_size;
    UINTN efi_descriptor_size;
} Memmap;

void get_memmap(Memmap *map);

void free_memmap(Memmap *map);
#endif
