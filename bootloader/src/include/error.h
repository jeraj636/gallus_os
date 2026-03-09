#ifndef ERROR_H
#define ERROR_H

#include <efi.h>
#include <efilib.h>

#include <stdbool.h>

// function prints error message and closes bootloader if exit == true
void error_handler(EFI_STATUS status, bool exit);

#endif