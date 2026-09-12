/*
 * adc.h
 *
 * Created: 08.09.2026 16:00:08
 *  Author: henrikwp
 */ 


#ifndef ADC_H_
#define ADC_H_


#ifdef F_CPU
#define F_CPU 4915200UL
#endif // F_CPU

// Timer limits for ADC
#define ADC_CLOCK_MIN 0.5f
#define ADC_CLOCK_MAX 5.0f




#endif /* ADC_H_ */