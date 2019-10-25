#define F_CPU 8000000 //8MHz clock


#include <avr/io.h>
#include <util/delay.h>  
  //led 1 pin 31 = PC6
  //led 2 pin	 = PC7
  
  //c button PD2
  //to set input with pullup, DDRX = 0, PORTX = 1
  
int main(void)
{
	DDRC |= 0xFF; //port c is outputs
	
	DDRD = 0x00; //all inputs
	PORTD |= ( 1<< PD2);//enable pullup
	while(1)
	{
		
		if(PIND & (1<<PD2))
		{
			PORTC |= ( 1 << PC7);
		}
		else
		{
			PORTC &= ~( 1 << PC7);
		}
		
		PORTC |= ( 1 << PC6 );
		_delay_ms(500);
		PORTC &= ~( 1 << PC6 );
		_delay_ms(500);
	}
}