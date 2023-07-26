#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "xtimer.h"
#include "driver_hx711.h"
#include "paho_mqtt_methods.h"
#include "app_params.h"

// MQTT
char* mqtt_topic_water;
char* mqtt_topic_food;

// hx711
hx711_t water_hx_dev;
hx711_t food_hx_dev;

void init_drivers()
{
    // MQTT
    mqtt_topic_water = malloc(sizeof(char)*(14+strlen(board_id)+1));
    mqtt_topic_food = malloc(sizeof(char)*(13+strlen(board_id)+1));
    sprintf(mqtt_topic_water, "iot/ADF/%s/water", board_id);
    sprintf(mqtt_topic_food, "iot/ADF/%s/food", board_id);
    init_paho_mqtt();
    
    // hx711
    hx711_setup(&water_hx_dev, &water_hx711_params);
    hx711_setup(&food_hx_dev, &food_hx711_params);
}

int main(void)
{
    init_drivers();

    while (1) {
        uint32_t start_time = xtimer_now_usec();

        /* Sample */
        int32_t water_value = hx711_get_units(&water_hx_dev);
        printf("water value %"PRIu32"\n", water_value);
        int32_t food_value = hx711_get_units(&food_hx_dev);
        printf("water value %"PRIu32"\n", food_value);

        publish_message(water_value, mqtt_topic_water)
        publish_message(food_value, mqtt_topic_food);

        uint32_t end_time = xtimer_now_usec();
        printf("Microseconds difference: %d\n", end_time-start_time); 
        start_time = end_time;

        xtimer_sleep(1);
    }

    return 0;
}