#include "command_handlers.h"
#include <stdbool.h>
#include "memap.h"
#include "error.h"
command known_commands[] = {
    {L"help", handle_help},
    {L"boot", handle_boot},
    {L"quit", handle_quit},
    {L"exit", handle_quit},
    {L"memmap", handle_memmap},
    {L"reboot", handle_reboot},
    {NULL, NULL},
};

void handle_help(EFI_SYSTEM_TABLE *system_table)
{
    EFI_STATUS status;
    ST = system_table;
    Print(L"Known commands:\n\r");
    for (int i = 0; known_commands[i].name != NULL; i++)
    {
        Print(known_commands[i].name);
        Print(L"\n\r");
        error_handler(status, true);
    }
}

void handle_quit(EFI_SYSTEM_TABLE *system_table)
{
    Print(L"Computher will power off!\n\rPress any key to continue.\n\r");

    // Wait for key press
    UINTN index;
    EFI_EVENT event = ST->ConIn->WaitForKey;
    ST->BootServices->WaitForEvent(1, &event, &index);

    ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL); // Exiting bootloder
}

void handle_reboot(EFI_SYSTEM_TABLE *system_table)
{
    // Wait for key press
    UINTN index;
    EFI_EVENT event = ST->ConIn->WaitForKey;
    ST->BootServices->WaitForEvent(1, &event, &index);

    ST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL); // Exiting bootloder
}

void handle_memmap(EFI_SYSTEM_TABLE *system_table)
{
    Memmap map;
    get_memmap(&map);
    UINT8 *ptr = (UINT8 *)map.efi_memory_map;
    UINTN map_end = (UINTN)map.efi_memory_map + map.efi_memory_map_size;

    while ((UINTN)ptr < map_end)
    {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *)ptr;

        EFI_PHYSICAL_ADDRESS add = descriptor->PhysicalStart;
        Print(L"Type: %-26s Address: 0x%lx - 0x%lx Pages: 0x%lx\n", MemoryTypeNames[descriptor->Type], descriptor->PhysicalStart, descriptor->PhysicalStart + descriptor->NumberOfPages * 4096, descriptor->NumberOfPages);
        ptr += map.efi_descriptor_size;
    }
    free_memmap(&map);
}
void handle_boot(EFI_SYSTEM_TABLE *system_table)
{
    EFI_STATUS status;
    ST = system_table;

    Print(L"Starting boot proces...\n\r");

    Print(L"\n\rGrtting CPUID...\n\r");

    // VendorID is stored in ebx/edx/ecx
    UINT32 eax, ebx, ecx, edx;
    asm volatile(
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0));
    Print(L"%x, %x, %x, %x\n\r", eax, ebx, ecx, edx);

    // Putting it to a char array
    CHAR8 vendor_id[13];

    *(UINT32 *)(vendor_id + 0) = ebx;
    *(UINT32 *)(vendor_id + 4) = edx;
    *(UINT32 *)(vendor_id + 8) = ecx;
    vendor_id[12] = '\0';

    Print(L"Vendor id: %a\n\r", vendor_id);

    // Getting number of CPUs
    asm volatile(
        "cpuid"
        : "=a"(eax), "=b"(ebx)
        : "a"(1));

    UINT32 number_of_CPUs = (ebx >> 16) & 0xff;

    Print(L"logical CPUS detected: %u\n\r", number_of_CPUs);

    // Saving the impotrant parts of memmap
    Print(L"Getting memory map...\n\r");

    Memmap map;
    get_memmap(&map);
    UINT8 *ptr = (UINT8 *)map.efi_memory_map;
    UINTN map_end = (UINTN)map.efi_memory_map + map.efi_memory_map_size;

    while ((UINTN)ptr < map_end)
    {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *)ptr;

        EFI_PHYSICAL_ADDRESS add = descriptor->PhysicalStart;

        Print(L"Type: %-26s Address: 0x%lx - 0x%lx Pages: 0x%lx\n", MemoryTypeNames[descriptor->Type], descriptor->PhysicalStart, descriptor->PhysicalStart + descriptor->NumberOfPages * 4096, descriptor->NumberOfPages);
        ptr += map.efi_descriptor_size;
    }
    free_memmap(&map);
}

command_func find_command(CHAR16 *function_name)
{
    for (int i = 0; known_commands[i].name != NULL; i++)
    {
        if (StrCmp(known_commands[i].name, function_name) == 0)
            return known_commands[i].func;
    }
    return NULL;
}