#include <stdio.h>
#include <inttypes.h>

#include "xtimer.h"
#include "driver_hx711.h"

const gpio_t gpio_dout = GPIO_PIN(0 ,1);
const gpio_t gpio_sck = GPIO_PIN(0, 2);
const uint16_t divider = 1;
const uint8_t read_times = 1;
const hx711_gain_t gain = CHANNEL_A_128;

const hx711_params_t hx711_params = {
    gpio_sck,
    gpio_dout,
    gain,
    read_times,
    divider
};

static hx711_t dev;

int main(void)
{

    xtimer_sleep(3);

    puts("HX711 test application\n");
    puts("+------------Initializing------------+");
    hx711_init(&dev, &hx711_params);
    hx711_power_up(&dev);
    puts("Initialization successful\n\n");

    puts("+--------Starting Measurements--------+");
    int32_t value_before = hx711_get_units(&dev);
    printf("value before taring: %"PRIu32"\n", value_before);
    hx711_tare(&dev);
    int32_t value_after = hx711_get_units(&dev);
    printf("value after taring: %"PRIu32"\n", value_after);

    while(1) {
        value_after = hx711_get_units(&dev);
        printf("value after taring: %"PRIu32"\n", value_after);

        xtimer_sleep(1);
    }
    
    return 0;
}