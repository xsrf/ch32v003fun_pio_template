#include "ch32v003fun.h"
#include <stdio.h>

#include "defines.h"

int main()
{
	SystemInit(); 


	// Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;

	// GPIO Push-Pull
	PORT->CFGLR &= ~(0xf<<(4*PIN));
	PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*PIN);

	while(1)
	{
		PORT->BSHR = (1<<PIN);
		Delay_Ms( DELAY );
		PORT->BSHR = (1<<(PIN+16));
		Delay_Ms( DELAY );
	}
}
