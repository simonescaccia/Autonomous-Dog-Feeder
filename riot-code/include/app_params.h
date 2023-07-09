#ifndef APP_PARAMS_H
#define APP_PARAMS_H

#include "driver_hx711.h"

// MQTT
#define MQTT_BRIDGE_IP "192.168.102.113"
extern const char board_id[];

// HX711
#define MAXIMUM_VALUE_LENGHT 4
extern const gpio_t gpio_dout;
extern const gpio_t gpio_sck;
extern const uint16_t divider;
extern const uint8_t read_times;
extern const hx711_gain_t gain;
extern const hx711_params_t hx711_params;

// generic
extern const uint16_t water_bowl_milliliters;
extern const uint16_t daily_food_grams;
extern const uint8_t number_of_meals_per_day;

#endif