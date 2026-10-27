/*
 * pwm.c
 *
 * Created: 13.10.2026
 * Author: alexanmf
 */

#include "sam.h"
#include "can.h"
#include "servo.h"
#include "adc.h"
#include <stdint.h>
#include <stdio.h>

#define SERVO_MIN_US      900u
#define SERVO_MAX_US     2099u
#define PWM_PERIOD_TICKS 52500u

static uint32_t us_to_ticks(uint32_t us)
{
	return (us * 2625u) / 1000u;
}

void init_pwm(void)
{
	// Enable PIOB and PWM clocks.
	PMC->PMC_PCER0 = (1u << ID_PIOB);
	PMC->PMC_PCER1 = (1u << (ID_PWM - 32));

	// PB13 -> Peripheral B (PWMH1).
	PIOB->PIO_ABSR |= PIO_ABSR_P13;
	PIOB->PIO_PDR = PIO_PDR_P13;

	// MCK/32 = 2.625 MHz; polarity to 1 makes CDTY the high pulse width.
	PWM->PWM_CH_NUM[1].PWM_CMR = (5u << 0) | PWM_CMR_CPOL;
	PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_PERIOD_TICKS;
	PWM->PWM_CH_NUM[1].PWM_CDTY = us_to_ticks(1500u);
}

void start_pwm(void)
{
	PWM->PWM_ENA = PWM_ENA_CHID1;
}

void set_pwm_pulse(uint32_t pulse_us)
{
	if (pulse_us < SERVO_MIN_US) pulse_us = SERVO_MIN_US;
	if (pulse_us > SERVO_MAX_US) pulse_us = SERVO_MAX_US;

	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = us_to_ticks(pulse_us);
}

void init_interrupt(void){
	
	PMC->PMC_PCER0 |= (1u << ID_TC1) | (1u << ID_TC2);
	
	TC0->TC_CHANNEL[1].TC_CMR =
	(2u << 0)  |  // TCCLKS = MCK/32
	(2u << 13) |  // WAVSEL = UP_RC
	(1u << 15);   // WAVE = 1 (waveform mode)
	
	TC0->TC_CHANNEL[2].TC_CMR =
	(2u << 0)  |  // TCCLKS = MCK/32
	(2u << 13) |  // WAVSEL = UP_RC
	(1u << 15);   // WAVE = 1 (waveform mode)
	
	TC0->TC_CHANNEL[1].TC_RC = 26250;  // Approximately 10 ms period    
	TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
	
	TC0->TC_CHANNEL[2].TC_RC = 10025;  // Approximately 4 ms period
	TC0->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;
	
	NVIC_EnableIRQ(TC1_IRQn);
	NVIC_EnableIRQ(TC2_IRQn);
	
	//Start the Timer Counter
	TC0->TC_CHANNEL[1].TC_CCR =
	TC_CCR_CLKEN | TC_CCR_SWTRG;
	
	TC0->TC_CHANNEL[2].TC_CCR =
	TC_CCR_CLKEN | TC_CCR_SWTRG;
}



void TC1_Handler(void)
{
	CanMsg msg;
	(void)TC0->TC_CHANNEL[1].TC_SR;
	if (can_rx(&msg) 
// 	&& !((int8_t)msg.byte[0] >= -5 &&
// 	(int8_t)msg.byte[0] <= 5)
	 ){
		//printf("x : %d \r \n",(int8_t) msg.byte[0]);
		servo_set((int8_t)msg.byte[0]);
	}
}

void TC2_Handler(void)
{
	(void)TC0->TC_CHANNEL[2].TC_SR;
	set_score();
	//printf("helo");
}