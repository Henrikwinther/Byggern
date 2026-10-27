#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "uart.h"
#include "sam.h"
#include "can.h"
#include "timer.h"
#include "adc.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(SystemCoreClock, 9600);
	
	init_adc();
	
    PIOB->PIO_PER = PIO_PB13; //Enable PB13 as GPIO

    PIOB->PIO_OER = PIO_PB13; //Enable PB13 as output

	init_pwm();
	start_pwm();
    
	init_interrupt();
	

    printf("Hello World\r\n");
	
  
    can_init(0);

	CanMsg msg;
	
	uint8_t previous_score = 0;
	
	printf("Score : %u\r\n", (unsigned int)previous_score);
	
    while (1)
    {
        if (score != previous_score) {
	        previous_score = score;
	        printf("Score : %u\r\n", (unsigned int)previous_score);
        }
    }
    
}