#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

#define PORT GPIOC
#define PIN 0
#define DELAY 100

int main()
{
	SystemInit();
	Delay_Ms(1); // Ensures USB re-enumeration after bootloader or reset; Spec demand >2.5µs ( TDDIS )

	// Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;

	// GPIO Push-Pull
	PORT->CFGLR &= ~(0xf<<(4*PIN));
	PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*PIN);

	usb_setup();
	while(1)
	{

		PORT->BSHR = (1<<PIN);
		Delay_Ms( DELAY );
		PORT->BSHR = (1<<(PIN+16));
		Delay_Ms( DELAY );
		
#if RV003USB_EVENT_DEBUGGING
		uint32_t * ue = GetUEvent();
		if( ue )
		{
			printf( "%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3] );
		}
#endif
	}
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
	if( endp == 1 )
	{
		// Mouse (4 bytes)
		static int i;
		static uint8_t tsajoystick[4] = { 0x00, 0x00, 0x00, 0x00 };
		i++;
		int mode = i >> 5;

		// Move the mouse right, down, left and up in a square.
		switch( mode & 3 )
		{
		case 0: tsajoystick[1] =  1; tsajoystick[2] = 0; break;
		case 1: tsajoystick[1] =  0; tsajoystick[2] = 1; break;
		case 2: tsajoystick[1] = -1; tsajoystick[2] = 0; break;
		case 3: tsajoystick[1] =  0; tsajoystick[2] =-1; break;
		}
		usb_send_data( tsajoystick, 4, 0, sendtok );
	}
	else if( endp == 2 )
	{
		// Keyboard (8 bytes)
		static int i;
		static uint8_t tsajoystick[8] = { 0x00 };
		usb_send_data( tsajoystick, 8, 0, sendtok );

		i++;

		// Press a Key every second or so.
		if( (i & 0x7f) == 0 )
		{
			tsajoystick[4] = 0x8; // 0x05 = "b"; 0x53 = NUMLOCK; 0x39 = CAPSLOCK;
		}
		else
		{
			tsajoystick[4] = 0;
		}
	}
	else
	{
		// If it's a control transfer, empty it.
		usb_send_empty( sendtok );
	}
}


