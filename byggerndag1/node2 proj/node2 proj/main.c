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
#include "motor.h"
#include "pwm.h"
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
	motor_init();
	init_encoder();
	can_init(0);
	init_interrupt();

	uint8_t previous_score = 0;
	int32_t previous_encoder_position = encoder_position();
	
	uint8_t ignore_first_score = 1;
	
	CanMsg score_msg = {0};
	score_msg.length = 1;
	score_msg.id = 10;
	score_msg.byte[0] = 0;
	
    while (1)
    {
// 		int32_t position = encoder_position();
// 		if (position != previous_encoder_position) {
// 			previous_encoder_position = position;
// 			printf("Encoder position : %ld\r\n", (long)position);
// 		}
		
        if (score != previous_score) {
			if (ignore_first_score) {
				score = 0;
				previous_score = 0;
				ignore_first_score = 0;
			} else {
				previous_score = score;
				score_msg.byte[0] = previous_score;
				can_tx(score_msg);
				printf("Score : %u\r\n", (unsigned int)previous_score);
			}
		}
    }
    
}