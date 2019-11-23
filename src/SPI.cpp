#include "SPI.h"

SPI::SPI()
{
	/* Set MOSI, SCK, SS as output*/
	//todo: enforce MISO input
	DDRB |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);
	
	PORTB |= (1<<MCUSS);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	//SPSR = (1<<SPI2X);
}
		
uint8_t SPI::transfer( uint8_t val)
{
	SPDR = val;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}