#include <stdint.h>


int kmain()
{
	uint16_t* buffer = (uint16_t *)0xB8000;
	uint16_t crka= 0x0f00;
	crka |= 'K';
	int j,i,k;
	*buffer = crka;
	while(1);
}


