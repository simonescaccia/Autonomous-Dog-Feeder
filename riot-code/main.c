#include <stdio.h>
#include <inttypes.h>

#include "xtimer.h"
#include "driver_hx711.h"

const gpio_t gpio_dout = GPIO_PIN(0 ,1);
const gpio_t gpio_sck = GPIO_PIN(0, 2);
const uint16_t divider = 1;
const uint8_t read_times = 200;

const hx711_params_t hx711_params = {
    gpio_sck,
    gpio_dout,
    read_times,
    divider
};

static hx711_t dev;

int main(void)
{
    xtimer_sleep(3);

    puts("Hello World!");

    hx711_init(&dev, &hx711_params);

    while(1) {
        printf("Value: %d\n", hx711_get_units(&dev, read_times));

        xtimer_sleep(1);
    }

    return 0;
}