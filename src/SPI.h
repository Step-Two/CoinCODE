#define MCUSS PB0
#define SCK PB1
#define MOSI PB2
#define MISO PB3


#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>


class SPI
{
  public:
	SPI();		
	uint8_t transfer( uint8_t val = 0);

};
#endif //_SPI_H_