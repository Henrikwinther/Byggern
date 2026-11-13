#ifndef TIMER1_H
#define TIMER1_H


/* Allowed ADC clock frequency range in megahertz. */
#define ADC_CLOCK_MIN_MHZ 0.5f
#define ADC_CLOCK_MAX_MHZ 5.0f

/* Configure Timer1 and return the actual clock frequency in MHz. */
float adc_clock_init(float f_mhz);

#endif
