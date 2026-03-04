#include <efi.h>
#include <efilib.h>

#include <stdbool.h>
#include <stdint.h>

#include "lib/memap.h"
#include "lib/error.h"

void handle_help(EFI_SYSTEM_TABLE *system_table);
void handle_boot(EFI_SYSTEM_TABLE *system_table);
void handle_quit(EFI_SYSTEM_TABLE *system_table);
void handle_reboot(EFI_SYSTEM_TABLE *system_table);
void handle_memmap(EFI_SYSTEM_TABLE *system_table);

typedef void (*command_func)(EFI_SYSTEM_TABLE *);
typedef struct
{
    CHAR16 *name;
    command_func func;
} command;

// It makes sense to make table of known commands

command known_commands[] = {
    {L"help", handle_help},
    {L"boot", handle_boot},
    {L"quit", handle_quit},
    {L"exit", handle_quit},
    {L"memmap", handle_memmap},
    {L"reboot", handle_reboot},
    {NULL, NULL},
};

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS status;
    EFI_INPUT_KEY key;

    ST = SystemTable;

    // Clearing screen
    status = ST->ConOut->ClearScreen(ST->ConOut);
    error_handler(status, true);

    // Printing startup message
    Print(L"Gallus OS bootloader\r\n>");
    error_handler(status, true);

    // Emptying console input buffer
    status = ST->ConIn->Reset(ST->ConIn, FALSE);
    error_handler(status, true);

    // Enabling cursor
    status = ST->ConOut->EnableCursor(ST->ConOut, TRUE);
    error_handler(status, true);

    // Buffer for command
    CHAR16 command[25] = L"\0";

    for (;;)
    {
        // Wait for pressed key
        while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY)
            ;

        CHAR16 pressed_key[4]; // Proccesing key
        SPrint(pressed_key, 4, L"%c", key.UnicodeChar);

        // Return is pressed
        if (key.UnicodeChar == L'\r')
        {
            // Go to new line
            Print(L"\r\n");

            bool command_exists = false;
            for (int i = 0; known_commands[i].name != NULL; i++)
                if (StrCmp(command, known_commands[i].name) == 0)
                {
                    known_commands[i].func(ST);
                    command_exists = true;
                }
            if (!command_exists)
            {
                Print(L"Command: ");
                Print(command);
                Print(L" does not exist!\n\r");
            }
            Print(L">");
            error_handler(status, true);

            // Clearing buffer
            command[0] = L'\0';
        }
        else if (key.UnicodeChar == L'\b') // Must be carefull to remove last character when pressing backspace
        {
            int vel = StrLen(command);
            if (vel >= 1)
            {
                command[vel - 1] = '\0';
                Print(pressed_key);
            }
        }
        else
        {
            Print(pressed_key);
            error_handler(status, true);
            StrCat(command, pressed_key);
        }
    }
    return status;
}

typedef struct
{
    uint64_t virtual_start;
    uint64_t real_start;
    uint64_t pages;
    uint32_t type;
    uint32_t owner;
} Memory_map_descriptor;

enum Memory_map_type
{
    CONVENTIONAL,
    RUNTIME_SERVICES_CODE,
    RUNTIME_SERVICES_DATA,

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
