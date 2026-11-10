#include "sam.h"
#include "motor.h"
#include "timer.h"

#define MOTOR_PWM_PERIOD_TICKS 131u
#define MOTOR_PHASE_PIN        PIO_PC23
#define MOTOR_COMMAND_LIMIT    100
#define MOTOR_TARGET_STEP_Q8   10000
#define MOTOR_POSITION_MIN     (-1352)
#define MOTOR_POSITION_MAX     1457
#define MOTOR_ERROR_LIMIT      3000
#define MOTOR_INTEGRAL_LIMIT   10000
#define MOTOR_COMMAND_TIMEOUT  200u

typedef enum {
	MOTOR_MODE_INCREMENTAL,
	MOTOR_MODE_DIRECT
} motor_mode_t;

#define MOTOR_CONTROL_MODE MOTOR_MODE_DIRECT

static int8_t joystick_command;
static int64_t target_position_q8;
static int32_t integral_error;
static uint8_t command_age;

static int32_t clamp(int32_t value, int32_t minimum, int32_t maximum)
{
	if (value < minimum) return minimum;
	if (value > maximum) return maximum;
	return value;
}

static void motor_apply(int8_t command)
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
		(magnitude * MOTOR_PWM_PERIOD_TICKS * 50u + (100u * 100u) / 2u) /
		(100u * 100u);
}

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

	joystick_command = 0;
	target_position_q8 = 0;
	integral_error = 0;
	command_age = MOTOR_COMMAND_TIMEOUT;
}

void motor_set(int8_t command)
{
	if (command > -8 && command < 8) {
		command = 0;
	}

	joystick_command = (int8_t)clamp(command, -MOTOR_COMMAND_LIMIT, MOTOR_COMMAND_LIMIT);
	command_age = 0;
}

void motor_update(void)
{
	int32_t position = encoder_position();
	int32_t position_q8 = (int64_t)position * 256;
	int32_t target_min_q8 = (int64_t)MOTOR_POSITION_MIN * 256;
	int32_t target_max_q8 = (int64_t)MOTOR_POSITION_MAX * 256;
	int32_t error_q8;
	int32_t error;
	int32_t candidate_integral;
	int32_t output;
	int64_t requested_target_q8;

	if (command_age < MOTOR_COMMAND_TIMEOUT) {
		command_age++;
	} else {
		joystick_command = 0;
	}

	switch (MOTOR_CONTROL_MODE) {
	case MOTOR_MODE_INCREMENTAL:
		requested_target_q8 = target_position_q8 -
			(int32_t)joystick_command * MOTOR_TARGET_STEP_Q8 / 1250;
		if (requested_target_q8 > target_max_q8) {
			target_position_q8 = target_max_q8;
			integral_error = 0;
		} else if (requested_target_q8 < target_min_q8) {
			target_position_q8 = target_min_q8;
			integral_error = 0;
		} else {
			target_position_q8 = requested_target_q8;
		}
		break;

	case MOTOR_MODE_DIRECT:
		if (joystick_command >= 0) {
			target_position_q8 =
				(int32_t)joystick_command * -target_max_q8 / MOTOR_COMMAND_LIMIT;
		} else {
			target_position_q8 =
				(int32_t)joystick_command * target_min_q8 / MOTOR_COMMAND_LIMIT;
		}
		break;
	}

	error_q8 = target_position_q8 - position_q8;
	if (error_q8 > (int64_t)MOTOR_ERROR_LIMIT * 256) {
		error = MOTOR_ERROR_LIMIT;
	} else if (error_q8 < -(int64_t)MOTOR_ERROR_LIMIT * 256) {
		error = -MOTOR_ERROR_LIMIT;
	} else {
		error = (int32_t)(error_q8 / 256);
	}

	candidate_integral = clamp(integral_error + error,
		-MOTOR_INTEGRAL_LIMIT, MOTOR_INTEGRAL_LIMIT);
	output = error + candidate_integral / 500;
	if ((output <= MOTOR_COMMAND_LIMIT && output >= -MOTOR_COMMAND_LIMIT) ||
		(output > MOTOR_COMMAND_LIMIT && error < 0) ||
		(output < -MOTOR_COMMAND_LIMIT && error > 0)) {
		integral_error = candidate_integral;
	}

	output = error + integral_error / 500;
	output = clamp(output, -MOTOR_COMMAND_LIMIT, MOTOR_COMMAND_LIMIT);

	motor_apply((int8_t)output);
}
