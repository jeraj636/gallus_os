#include "stdio.h"
#include "string.h"

int obrni_int(int st)
{
    bool je_neg;
    if (st == 0)
        return 0;
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
void daj_int_v_buffer(char *buffer, int o_st, size_t *dol)
{
    int st = obrni_int(o_st);
    int st_nul = 0;
    if (o_st != 0)
        while (o_st % 10 == 0)
        {
            st_nul++;
            o_st /= 10;
        }

    if (st == 0)
    {
        buffer[*dol] = '0';
        buffer[++(*dol)] = '\0';
    }
    if (st < 0)
    {
        buffer[*dol] = '-';
        buffer[++(*dol)] = '\0';
        st *= -1;
    }
    while (st > 0)
    {
        char c = st % 10;
        st /= 10;
        c += '0';
        buffer[*dol] = c;
        buffer[++(*dol)] = '\0';
    }
    for (int i = 0; i < st_nul; i++)
    {
        buffer[*dol] = '0';
        buffer[++(*dol)] = '\0';
    }
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
                int o_st = va_arg(argumenti, int);
                daj_int_v_buffer(izpis, o_st, &dol);
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