#include "memap.h"
#include <stdbool.h>
#include "error.h"
void get_memmap(Memmap *map)
{
    EFI_STATUS status;
    EFI_MEMORY_DESCRIPTOR *efi_memory_map = NULL;

    UINTN efi_memory_map_size = 0;
    UINTN efi_map_key;
    UINTN efi_descriptor_size;
    UINT32 efi_descriptor_version;

    // Getting size of memory map
    status = ST->BootServices->GetMemoryMap(&efi_memory_map_size,
                                            efi_memory_map,
                                            &efi_map_key,
                                            &efi_descriptor_size,
                                            &efi_descriptor_version);

    error_handler(status, false);
    efi_memory_map_size += 2 * efi_descriptor_size; // Setting the size
    // Alocating memory for memory map
    status = ST->BootServices->AllocatePool(EfiLoaderData, efi_memory_map_size, (VOID **)&efi_memory_map);
    status = ST->BootServices->GetMemoryMap(&efi_memory_map_size,
                                            efi_memory_map,
                                            &efi_map_key,
                                            &efi_descriptor_size,
                                            &efi_descriptor_version);

    error_handler(status, true);
    map->efi_memory_map = efi_memory_map;
    map->efi_memory_map_size = efi_memory_map_size;
    map->efi_descriptor_size = efi_descriptor_size;
}
void free_memmap(Memmap *map)
{
    ST->BootServices->FreePool((void *)map->efi_memory_map);
}