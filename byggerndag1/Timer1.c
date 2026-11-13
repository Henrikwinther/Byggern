#include "Timer1.h"

#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 4915200UL
#endif // F_CPU


/* Select the timer compare value that best matches the requested ADC clock. */
float adc_clock_init(float f_mhz)
{
    /* Reject requests outside the ADC's supported clock range. */
    if (f_mhz < ADC_CLOCK_MIN_MHZ || f_mhz > ADC_CLOCK_MAX_MHZ) {
        return 0.0f; /* outside what the MAX156 tolerates */
    }
 
    /*
     * Walk OCR upward until the produced frequency drops below the
     * MAX156 minimum; among the legal candidates, keep the one
     * closest to the request. The candidate set is derived from the
     * constraint itself, so nothing here changes if F_CPU does.
     */
    uint16_t best_ocr = 0;
    float best_err = -1.0f;
 
    /* Search every legal compare value and keep the closest frequency. */
    for (uint16_t ocr = 0;; ocr++) {
        float f = (float)F_CPU / (2.0f * (float)(ocr + 1)) / 1e6f;
        if (f < ADC_CLOCK_MIN_MHZ) {
            break; /* this and all larger OCRs are out of spec */
        }
 
        float err = (f > f_mhz) ? (f - f_mhz) : (f_mhz - f);
        if (best_err < 0.0f || err < best_err) {
            best_err = err;
            best_ocr = ocr;
        }
    }
 
    /* Set the output pin and start Timer1 in CTC toggle mode. */
    DDRD |= (1 << PD5); /* OC1A must be an output for the toggle
                           to appear on the pin */
 
    /* Configure fully before starting: OCR and waveform mode first,
       clock select last, so the timer never runs with a stale TOP. */
    OCR1A  = best_ocr;
    TCCR1A = (1 << COM1A0);              /* toggle OC1A on match    */
    TCCR1B = (1 << WGM12) | (1 << CS10); /* CTC mode 4, prescaler 1 */
 
    return (float)F_CPU / (2.0f * (float)(best_ocr + 1)) / 1e6f;
}
