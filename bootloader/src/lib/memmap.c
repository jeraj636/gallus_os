#include "memap.h"
#include <stdbool.h>
#include "error.h"

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

void get_memmap(Memmap *map)
{
    EFI_STATUS status;
    EFI_MEMORY_DESCRIPTOR *efi_memory_map = NULL;

    UINTN efi_memory_map_size = 0;
    UINTN efi_map_key;
    UINTN efi_descriptor_size;
    UINT32 efi_descriptor_version;

    // Getting size of memory map
    efi_memory_map_size = 0;
    efi_memory_map = NULL;
    status = ST->BootServices->GetMemoryMap(&efi_memory_map_size,
                                            efi_memory_map,
                                            &efi_map_key,
                                            &efi_descriptor_size,
                                            &efi_descriptor_version);
    efi_memory_map_size += 2 * efi_descriptor_size;

    // Alocating memory for memory map
    status = ST->BootServices->AllocatePool(EfiLoaderData, efi_memory_map_size, (VOID **)&efi_memory_map);
    error_handler(status, false);

    status = ST->BootServices->GetMemoryMap(&efi_memory_map_size,
                                            efi_memory_map,
                                            &efi_map_key,
                                            &efi_descriptor_size,
                                            &efi_descriptor_version);

    error_handler(status, false);
    map->efi_memory_map = efi_memory_map;
    map->efi_memory_map_size = efi_memory_map_size;
    map->efi_descriptor_size = efi_descriptor_size;
}
void free_memmap(Memmap *map)
{
    ST->BootServices->FreePool((void *)map->efi_memory_map);
}