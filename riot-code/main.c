#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "xtimer.h"
#include "driver_hx711.h"
#include "paho_mqtt_methods.h"


#define MAXIMUM_VALUE_LENGHT 10

static const char board_id[] = "1";

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
    return;

    puts("HX711 test application\n");
    puts("+------------Initializing------------+");
    hx711_init(&dev, &hx711_params);
    hx711_power_up(&dev);
    puts("Initialization successful\n\n");

    puts("+--------Starting Measurements--------+");
    int32_t value_before = hx711_get_units(&dev);
    printf("value before taring: %"PRIu32"\n", value_before);
    hx711_tare(&dev);

    int32_t value_after;
    while (1) {
        value_after = hx711_get_units(&dev);
        printf("value after taring: %"PRIu32"\n", value_after);

        xtimer_sleep(1);
    }
}

int main(void)
{

    // Topics
    char* MQTT_TOPIC_WATER = sprintf("iot/ADF/%s/water", board_id);
    char* MQTT_TOPIC_FOOD = sprintf("iot/ADF/%s/food", board_id);

    init_paho_mqtt();
    _sample();

    char* con_list[2] = {"cmd_con", MQTT_BRIDGE_IP};
    char** con_argv = (char**)&con_list;
    while (cmd_con(2, con_argv) < 0) {
        /* Wait for the WiFi connection */
        xtimer_sleep(1);
    }

    /* Sample */
    float value = 0.5;
    while (1) {
        /* Convert the value to string */
        char* str_value = malloc(sizeof(char*)*MAXIMUM_VALUE_LENGHT);
        int chars = fmt_float(str_value, value, 2);
        str_value[chars] = '\0';
        /* Concat the string with the deviceId */
        char* message = malloc(sizeof(char*)*(strlen(board_id)+strlen(str_value)+2));
        sprintf(message, "%s,%s", board_id, str_value);
        /* Publish the value */
        char* pub_list[3] = {"cmd_pub", MQTT_TOPIC_WATER, message};
        char** pub_argv = (char**)&pub_list;
        cmd_pub(3, pub_argv);
        /* Free space */
        free(str_value);
        free(message);
        
        xtimer_sleep(10);
        value += 1;
    }

    return 0;
}