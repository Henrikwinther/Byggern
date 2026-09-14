#ifndef TIMER1_H
#define TIMER1_H


/*Klokke limits i Mhz*/
#define ADC_CLOCK_MIN_MHZ 0.5f
#define ADC_CLOCK_MAX_MHZ 5.0f

/*Returnerer den sanne klokke frekvensen som er satt
på Timer1. */
float adc_clock_init(float f_mhz);

#endif
