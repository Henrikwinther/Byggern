/*
 * timer.c
 *
 * Created: 13.10.2026
 * Author: alexanmf
 */

#include "sam.h"
#include "can.h"
#include "motor.h"
#include "servo.h"
#include "adc.h"
#include <stdint.h>

#define ENCODER_PINS  PIO_PC25 | PIO_PC26
#define BUTTON_OUTPUT_PIN PIO_PB15

static volatile int8_t pending_servo_command;

void init_encoder(void)
{
	// Enable PIOC and TC2 channel 0 (peripheral ID_TC6) clocks.
	PMC->PMC_PCER0 |= (1u << ID_PIOC);
	PMC->PMC_PCER1 |= (1u << (ID_TC6 - 32));

	// PC25 = TIOA6 and PC26 = TIOB6, both on peripheral B.
	PIOC->PIO_ABSR |= ENCODER_PINS;
	PIOC->PIO_PDR |= ENCODER_PINS;

	// Quadrature decoder is configured for the whole TC2 block.
	TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;

	// The position counter on channel 0 must use XC0 in quadrature mode.
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_position(void)
{
	return (int32_t)TC2->TC_CHANNEL[0].TC_CV;
}

void init_interrupt(void)
{
	// Activate clocks for PIOA, TC0 channel 0, TC3, and TC4.
	PMC->PMC_PCER0 |= (1u << ID_PIOA) | (1u << ID_TC0) |
		(1u << ID_TC3) | (1u << ID_TC4);

	// PA11 is a GPIO output for the CAN button signal.
	PIOB->PIO_PER |= BUTTON_OUTPUT_PIN;
	PIOB->PIO_CODR = BUTTON_OUTPUT_PIN;
	PIOB->PIO_OER |= BUTTON_OUTPUT_PIN;

	// TC0 (TC0 channel 0): 20 ms servo update.
	TC0->TC_CHANNEL[0].TC_CMR =
	(2u << 0)  |  // TCCLKS = MCK/32
	(2u << 13) |  // WAVSEL = UP_RC
	(1u << 15);   // WAVE = 1

	TC0->TC_CHANNEL[0].TC_RC = 52500;
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;

	// TC3 (TC1 channel 0): ~2 ms position-control update.
	TC1->TC_CHANNEL[0].TC_CMR =
	(2u << 0)  |  // TCCLKS = MCK/32
	(2u << 13) |  // WAVSEL = UP_RC
	(1u << 15);   // WAVE = 1

	TC1->TC_CHANNEL[0].TC_RC = 1000;
	TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;

	// TC4 (TC1 channel 1): ~4 ms.
	TC1->TC_CHANNEL[1].TC_CMR =
	(2u << 0)  |  // TCCLKS = MCK/32
	(2u << 13) |  // WAVSEL = UP_RC
	(1u << 15);   // WAVE = 1

	TC1->TC_CHANNEL[1].TC_RC = 10025;
	TC1->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;

	NVIC_EnableIRQ(TC0_IRQn);
	NVIC_EnableIRQ(TC3_IRQn);
	NVIC_EnableIRQ(TC4_IRQn);

	// Start timers.
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void TC0_Handler(void)
{
	(void)TC0->TC_CHANNEL[0].TC_SR;
	servo_set(pending_servo_command);
}

void TC3_Handler(void)
{
	CanMsg msg;
	(void)TC1->TC_CHANNEL[0].TC_SR;

	if (can_rx(&msg)) {
			motor_set((int8_t)msg.byte[0]);
			pending_servo_command = (int8_t)msg.byte[1];
			
			if (msg.byte[2] || msg.byte[3]) {
				PIOB->PIO_CODR = BUTTON_OUTPUT_PIN;
				
			} else {
				PIOB->PIO_SODR = BUTTON_OUTPUT_PIN;
			}
		
	}
	motor_update();
}

void TC4_Handler(void)
{
	(void)TC1->TC_CHANNEL[1].TC_SR;
	set_score();
}