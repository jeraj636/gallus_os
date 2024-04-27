#include "stdio.h"
#include "string.h"

int obrni_int(int st)
{
    bool je_neg;

    if (st < 0)
    {
        je_neg = true;
        st *= -1;
    }
    else
        je_neg = false;

    int t = 0;
    while (st > 0)
    {
        t += st % 10;
        st /= 10;
        t *= 10;
    }
    return (je_neg) ? t / -10 : t / 10;
}

void printf(const char *niz, ...)
{
    va_list argumenti;
    va_start(argumenti, niz);
    char izpis[512];
    izpis[0] = '\0';
    size_t dol = 0;
    for (size_t i = 0; niz[i] != '\0'; ++i)
    {
        if ('%' == niz[i])
            switch (niz[++i])
            {
            case 'i':
            case 'd':
                int st = obrni_int(va_arg(argumenti, int));
                if (st < 0)
                {
                    izpis[dol] = '-';
                    izpis[++dol] = '\0';
                    st *= -1;
                }
                while (st > 0)
                {
                    char c = st % 10;
                    st /= 10;
                    c += '0';
                    izpis[dol] = c;
                    izpis[++dol] = '\0';
                }
                break;
            }
        else
        {
            izpis[dol] = niz[i];
            izpis[++dol] = '\0';
        }
    }
    vga_izpisi_niz(izpis);
    va_end(argumenti);
}