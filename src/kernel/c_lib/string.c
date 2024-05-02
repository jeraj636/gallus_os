#include "string.h"

size_t strlen(const char *niz)
{
    size_t i = 0;
    for (; niz[i++] != '\0';)
        ;
    return --i;
}

void strcat(char *niz1, const char *niz2)
{
    size_t dol = strlen(niz1);
    for (size_t i = 0; niz2[i] != '\0'; i++)
        niz1[i + dol] = niz2[i];
    niz1[dol + strlen(niz2)] = '\0';
}

void memcpy(void *dest, void *src, size_t vel)
{
    for (size_t i = 0; i < vel; i++)
    {
        ((char *)dest)[i] = ((char *)src)[1];
    }
}
