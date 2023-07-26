#ifndef APP_PARAMS_H
#define APP_PARAMS_H

#include "driver_hx711.h"

// MQTT
#define MQTT_BRIDGE_IP "192.168.1.7"
extern const char board_id[];

// HX711
#define MAXIMUM_VALUE_LENGHT 4
extern const gpio_t water_hx_gpio_dout;
extern const gpio_t water_hx_gpio_sck;
extern const uint16_t water_hx_divider;
extern const uint8_t water_hx_read_times;
extern const hx711_gain_t water_hx_gain;
extern const hx711_params_t water_hx711_params;

extern const gpio_t food_hx_gpio_dout;
extern const gpio_t food_hx_gpio_sck;
extern const uint16_t food_hx_divider;
extern const uint8_t food_hx_read_times;
extern const hx711_gain_t food_hx_gain;
extern const hx711_params_t food_hx711_params;

// generic
extern const uint16_t water_bowl_milliliters;
extern const uint16_t daily_food_grams;
extern const uint8_t number_of_meals_per_day;

#endif