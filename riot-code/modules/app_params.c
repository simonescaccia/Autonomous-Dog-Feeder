#include "app_params.h"

const char board_id[] = "1";

const gpio_t gpio_dout = GPIO_PIN(0 ,6);
const gpio_t gpio_sck = GPIO_PIN(0, 7);
const uint16_t divider = 362;
const uint8_t read_times = 1;
const hx711_gain_t gain = CHANNEL_A_128;

const hx711_params_t hx711_params = {
    gpio_sck,
    gpio_dout,
    gain,
    read_times,
    divider
};

const uint16_t water_bowl_milliliters = 500;
const uint16_t daily_food_grams = 400;
const uint8_t number_of_meals_per_day = 2;