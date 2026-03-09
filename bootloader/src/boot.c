#include <efi.h>
#include <efilib.h>

#include <stdbool.h>
#include <stdint.h>

#include "command_handlers.h"
#include "memap.h"
#include "error.h"

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

            // Find a funtion that must be called
            command_func function = find_command(command);
            if (function == NULL)
            {
                Print(L"Command: ");
                Print(command);
                Print(L" does not exist!\n\r");
            }
            else
            {
                function(ST); // Calling function
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
        else // backspace is not pressed
        {
            Print(pressed_key);
            error_handler(status, true);
            StrCat(command, pressed_key);
        }
    }
    return status;
}
