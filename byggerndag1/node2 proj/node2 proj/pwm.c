/*
 * pwm.c
 *
 * Created: 13.10.2026
 * Author: alexanmf
 */

#include "sam.h"
#include "pwm.h"
#include <stdint.h>

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
