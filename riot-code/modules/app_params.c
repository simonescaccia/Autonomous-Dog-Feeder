#include "app_params.h"

const char board_id[] = "1";

// hx711
const gpio_t water_hx_gpio_dout = GPIO_PIN(0 ,6);
const gpio_t water_hx_gpio_sck = GPIO_PIN(0, 7);
const uint16_t water_hx_divider = 494;
const uint8_t water_hx_read_times = 1;
const hx711_gain_t water_hx_gain = CHANNEL_A_128;

const hx711_params_t water_hx711_params = {
    water_hx_gpio_sck,
    water_hx_gpio_dout,
    water_hx_gain,
    water_hx_read_times,
    water_hx_divider
};

const gpio_t food_hx_gpio_dout = GPIO_PIN(0 ,5);
const gpio_t food_hx_gpio_sck = GPIO_PIN(0, 4);
const uint16_t food_hx_divider = 479;
const uint8_t food_hx_read_times = 1;
const hx711_gain_t food_hx_gain = CHANNEL_A_128;

const hx711_params_t food_hx711_params = {
    food_hx_gpio_sck,
    food_hx_gpio_dout,
    food_hx_gain,
    food_hx_read_times,
    food_hx_divider
};

// servo

const servo_pwm_params_t my_servo_pwm_params = {
    .pwm = SERVO_PWM_PARAM_DEV,
    .freq = SERVO_PWM_PARAM_FREQ,
    .res = SERVO_PWM_PARAM_RES
};

const servo_params_t switch_servo_params = {
    .pwm = &my_servo_pwm_params, 
    .pwm_chan = SWITCH_SERVO_PWM_PARAM_CHAN,
    .min_us = SERVO_PARAM_MIN_US,
    .max_us = SERVO_PARAM_MAX_US
};

const servo_params_t shake_servo_params = {
    .pwm = &my_servo_pwm_params, 
    .pwm_chan = SHAKE_SERVO_PWM_PARAM_CHAN,
    .min_us = SERVO_PARAM_MIN_US,
    .max_us = SERVO_PARAM_MAX_US
};

// generic
const uint16_t water_bowl_milliliters = 500;
const uint16_t daily_food_grams = 400;
const uint8_t number_of_meals_per_day = 2;