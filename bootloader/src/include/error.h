#ifndef ERROR_H
#define ERROR_H

#include <efi.h>
#include <efilib.h>

#include <stdbool.h>
void error_handler(EFI_STATUS status, bool exit);

#endif