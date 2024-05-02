#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char *niz);
void strcat(char *niz1, const char *niz2);
void memcpy(void *dest, void *src, size_t vel);
#endif