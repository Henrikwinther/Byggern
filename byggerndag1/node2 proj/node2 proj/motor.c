#include "sam.h"
#include "motor.h"

#define MOTOR_PWM_PERIOD_TICKS 131u
#define MOTOR_PHASE_PIN        PIO_PC23

void motor_init(void)
{
	// Enable PIOB, PIOC, and PWM peripheral clocks.
	PMC->PMC_PCER0 |= (1u << ID_PIOB) | (1u << ID_PIOC);
	PMC->PMC_PCER1 |= (1u << (ID_PWM - 32));

	// PB12 -> Peripheral B (PWMH0).
	PIOB->PIO_ABSR |= PIO_PB12;
	PIOB->PIO_PDR |= PIO_PB12;

	// PC23 -> GPIO output for the driver's PHASE/DIR input.
	PIOC->PIO_PER |= MOTOR_PHASE_PIN;
	PIOC->PIO_CODR = MOTOR_PHASE_PIN;
	PIOC->PIO_OER |= MOTOR_PHASE_PIN;

	// MCK/32 = 2.625 MHz; 131 ticks gives approximately 20 kHz.
	PWM->PWM_CH_NUM[0].PWM_CMR = (5u << 0) | PWM_CMR_CPOL;;
	PWM->PWM_CH_NUM[0].PWM_CPRD = MOTOR_PWM_PERIOD_TICKS;
	PWM->PWM_CH_NUM[0].PWM_CDTY = 0u;
	PWM->PWM_ENA = PWM_ENA_CHID0;
}

void motor_set(int8_t command)
{
	int16_t signed_command = command;
	uint32_t magnitude;

	if (signed_command < 0) {
		PIOC->PIO_SODR = MOTOR_PHASE_PIN;
		magnitude = (uint32_t)(-signed_command);
	} else {
		PIOC->PIO_CODR = MOTOR_PHASE_PIN;
		magnitude = (uint32_t)signed_command;
	}

	PWM->PWM_CH_NUM[0].PWM_CDTYUPD =
		(magnitude * MOTOR_PWM_PERIOD_TICKS * 50u ) / (128u * 100u);
}
