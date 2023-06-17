#include <stdio.h>
#include <inttypes.h>

#include "xtimer.h"
#include "driver_hx711.h"
#include "paho_mqtt.h"

#define MQTT_BRIDGE_IP "192.168.194.113"

const gpio_t gpio_dout = GPIO_PIN(0 ,1);
const gpio_t gpio_sck = GPIO_PIN(0, 2);
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

static hx711_t dev;

static void _sample (void) {
    puts("HX711 test application\n");
    puts("+------------Initializing------------+");
    hx711_init(&dev, &hx711_params);
    hx711_power_up(&dev);
    puts("Initialization successful\n\n");

    puts("+--------Starting Measurements--------+");
    int32_t value_before = hx711_get_units(&dev);
    printf("value before taring: %"PRIu32"\n", value_before);
    hx711_tare(&dev);

    while (1) {
        value_after = hx711_get_units(&dev);
        printf("value after taring: %"PRIu32"\n", value_after);

        xtimer_sleep(1);
    }
}

int main(void)
{

    init_paho_mqtt();

    char* list[1] = {"cmd_con", MQTT_BRIDGE_IP};
    char** argv = (char**)&list;
    while (cmd_con(1, argv) < 0) {
        /* Wait for the WiFi connection */
        xtimer_sleep(1);
    }

    float value = 0.5;
    while (1) {
        
        
        
        
        xtimer_sleep(5);
        value += 1;
    }

    return 0;
}