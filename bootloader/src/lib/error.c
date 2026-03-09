#include "error.h"

void error_handler(EFI_STATUS status, bool exit)
{
    if (EFI_ERROR(status))
    {
        // Printing error message
        Print(L"\r\n[!] Error: (Code: %r)\r\n", status);
        // Wait for key press
        UINTN index;
        EFI_EVENT event = ST->ConIn->WaitForKey;
        ST->BootServices->WaitForEvent(1, &event, &index);

        if (exit)
        {

            // Exiting bootloder
            ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL); // Exiting bootloder
        }
    }
}