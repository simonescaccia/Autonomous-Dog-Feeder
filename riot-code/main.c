#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "xtimer.h"
#include "driver_hx711.h"
#include "paho_mqtt_methods.h"
#include "app_params.h"

int main(void)
{
    // MQTT
    char* MQTT_TOPIC_WATER = malloc(sizeof(char)*(14+strlen(board_id)+1));
    char* MQTT_TOPIC_FOOD = malloc(sizeof(char)*(13+strlen(board_id)+1));
    sprintf(MQTT_TOPIC_WATER, "iot/ADF/%s/water", board_id);
    sprintf(MQTT_TOPIC_FOOD, "iot/ADF/%s/food", board_id);

    // hx711
    static hx711_t water_dev;

    init_paho_mqtt();
    hx711_setup(&water_dev, &hx711_params);

    while (1) {
        uint32_t start_time = xtimer_now_usec();
        /* Sample */
        int32_t value = hx711_get_units(&water_dev);
        printf("value %"PRIu32"\n", value);
        /* Convert the value to string */
        char* str_value = malloc(sizeof(char*)*MAXIMUM_VALUE_LENGHT);
        sprintf(str_value, "%" PRIu32 "", value);
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

        uint32_t end_time = xtimer_now_usec();
        printf("Microseconds difference: %d\n", end_time-start_time); 
        start_time = end_time;

        xtimer_sleep(5);
    }

    return 0;
}