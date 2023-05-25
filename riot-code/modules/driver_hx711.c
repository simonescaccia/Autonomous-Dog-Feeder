#include "driver_hx711.h"
#include "periph/gpio_util.h"
#include <stdio.h>

static int i = 0;

static int32_t hx711_read(hx711_t *dev)
{
   union {
        int32_t i32;
        struct {
            uint8_t data[3];
            uint8_t fill_byte;
        } u8;
    } value;

    value.i32 = 0;

    gpio_t sck = dev->params.sck;
    gpio_t dout = dev->params.dout;

    value.u8.data[2] = gpio_util_shiftin(dout, sck);
    value.u8.data[1] = gpio_util_shiftin(dout, sck);
    value.u8.data[0] = gpio_util_shiftin(dout, sck);

    if (value.u8.data[2] & 0x80) {
        value.u8.fill_byte = 0xFF;
    } else {
        value.u8.fill_byte = 0x00;
    }

    value.i32 = (value.u8.fill_byte << 24) | (value.u8.data[2] << 16) | (value.u8.data[1] << 8) | value.u8.data[0];

    if (i == 0) {
        printf("fill_byte: %"PRIu8"", value.u8.fill_byte);
        printf("data[2]: %"PRIu8"", value.u8.data[2]);
        printf("data[1]: %"PRIu8"", value.u8.data[1]);
        printf("data[0]: %"PRIu8"", value.u8.data[0]);
        printf("i32: %"PRIu32"", value.i32);
        i++;
    }

    return value.i32;
}

static void hx711_power_up(hx711_t *dev) 
{
    gpio_clear(dev->params.sck);
}

void hx711_init (hx711_t *dev, const hx711_params_t *params) 
{
    dev->params = *params;
    dev->offset = 0;

    gpio_init(params->sck, GPIO_OUT);
    gpio_init(params->dout, GPIO_IN);

    hx711_power_up(dev);
}

static int32_t hx711_read_average(hx711_t *dev, uint8_t times)
{
    int32_t sum = 0;
    uint8_t i = times;

    while (i--) {
        sum += hx711_read(dev);
    }

    return sum / times;
}

static int32_t hx711_get_value(hx711_t *dev, uint8_t times)
{
    return hx711_read_average(dev, times) - dev->offset;
}

int32_t hx711_get_units(hx711_t *dev, int8_t times)
{
    return hx711_get_value(dev, times) / dev->params.divider;
}