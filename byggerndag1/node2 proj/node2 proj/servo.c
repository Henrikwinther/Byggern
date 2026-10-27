/*
 * servo.c
 *
 * Created: 13.10.2026 14:18:08
 *  Author: alexanmf
 */ 

#include "servo.h"
#include "pwm.h"

#define SERVO_CENTER_US 1500

void servo_set(int8_t command)
{
	int32_t pulse = SERVO_CENTER_US + (int32_t)command * 6;
	set_pwm_pulse((uint32_t)pulse);
}
