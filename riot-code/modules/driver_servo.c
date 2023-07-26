/**
 * Define the PWM pins in boards/esp32s3-devkit/include/periph_conf.h
 * define PWM0_GPIOS  { GPIO19, GPIO20, GPIO21 } 
 * stands for PWM_DEV0
 * Channel 0: GPIO19, Channel 1: GPIO20, Channel 2: GPIO21
 * 
 * For servo 0°-90°, use:
 * #define SERVO_MIN   (1000U)
 * #define SERVO_MAX   (2000U)
 * 
 * For servo 0°-180°, use:
 * #define SERVO_MIN   (500U)
 * #define SERVO_MAX   (2500U)
 * 
 * 
*/

#include <stdio.h>
#include "driver_servo.h"

#define POS0 0
#define POS1 UINT8_MAX

void servo_setup(servo_t *dev, const servo_params_t *params)
{
    servo_init(dev, params);
}

void servo_on(servo_t *dev)
{
    servo_set(dev, POS0);
}

void servo_off(servo_t *dev)
{
    servo_set(dev, POS1);
}