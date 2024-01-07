#include "ch32v003fun.h"
#include "ch32v003_touch.h"
#include <stdio.h>

// D6 on nanoCH32V003
// C0 on CH32V003FUN

#define PORT GPIOC
#define PIN 0
#define DELAY 100

int main()
{
	SystemInit(); 


	// Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;

	// GPIO Push-Pull
	PORT->CFGLR &= ~(0xf<<(4*PIN));
	PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*PIN);

	InitTouchADC();
	uint32_t touchVal;
	touchVal = ReadTouchPin(GPIOC,4,0,10);

	while(1)
	{
		PORT->BSHR = (1<<PIN);
		Delay_Ms( DELAY );
		PORT->BSHR = (1<<(PIN+16));
		Delay_Ms( DELAY );
	}
}
