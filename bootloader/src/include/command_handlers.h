#ifndef COMMAND_HANDLERS
#define COMMAND_HANDLERS

#include <efi.h>
#include <efilib.h>

// typediniton of pointer to bootloder funtion
typedef void (*command_func)(EFI_SYSTEM_TABLE *);

// this struct will be used in table of known commands
typedef struct
{
    CHAR16 *name;
    command_func func;
} command;

// Definitions of all known commands in our bootloader
void handle_help(EFI_SYSTEM_TABLE *system_table);
void handle_boot(EFI_SYSTEM_TABLE *system_table);
void handle_quit(EFI_SYSTEM_TABLE *system_table);
void handle_reboot(EFI_SYSTEM_TABLE *system_table);
void handle_memmap(EFI_SYSTEM_TABLE *system_table);

// This will be table of all known commands
extern command known_commands[];

// function that finds pointer of function
command_func find_command(CHAR16 *function_name);

#endif