#include "sam.h"
#include "adc.h"
#include <stdio.h>
#include <stdbool.h>


volatile uint8_t score = 0;

void init_adc(void)
{
    // enable clock
    PMC->PMC_PCER1 = 1 << (ID_ADC - 32);
    
    //Reset
    ADC->ADC_CR = ADC_CR_SWRST;

    // MCK = 84 MHz, f_ADC = MCK / ((PRESCAL+1)*2) = 10,5 MHz (max 20 MHz)
    ADC->ADC_MR = ADC_MR_PRESCAL(3)
                | ADC_MR_STARTUP_SUT64
                | ADC_MR_TRACKTIM(15)
                | ADC_MR_TRANSFER(2)
                | ADC_MR_FREERUN_ON;      // continous conversion

    // activate channel
    ADC->ADC_CHER = 1 << 0;
}

uint16_t adc_read(void)
{
    return (ADC->ADC_CDR[0] & 0x0FFF);
}


void set_score(void)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < 8; i++) {
        sum += adc_read();	
    }
	
    uint16_t avg = (uint16_t)(sum / 8);

    bool blocked = avg > 2000;

    static bool candidate_blocked = false;
    static bool stable_blocked = false;
    static bool initialized = false;
    static uint8_t candidate_count = 0;

    if (blocked != candidate_blocked) {
	    candidate_blocked = blocked;
	    candidate_count = 1;
	    } else if (candidate_count < 4) {
	    candidate_count++;
    }
    if (candidate_count >= 4) {
	    if (!initialized) {
		    stable_blocked = candidate_blocked;
		    initialized = true;
	    } else if (candidate_blocked != stable_blocked) {
		    stable_blocked = candidate_blocked;

		    if (stable_blocked) {
			    score++;
		    }
	    }
    }
}