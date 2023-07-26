#include "app_params.h"
#include "water_pump.h"

void water_pump_setup(void) {
    gpio_init(RELAY_PIN, GPIO_OUT);
}

void water_pump_off(void) {
    gpio_clear(RELAY_PIN);
}

void water_pump_on(void) {
    gpio_set(RELAY_PIN);
}