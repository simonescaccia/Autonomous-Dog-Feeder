#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "xtimer.h"
#include "driver_hx711.h"
#include "driver_servo.h"
#include "water_pump.h"
#include "slide_switch.h"
#include "paho_mqtt_methods.h"
#include "app_params.h"

// MQTT
char* mqtt_topic_water;
char* mqtt_topic_food;

// hx711
hx711_t water_hx_dev;
hx711_t food_hx_dev;

// servo
servo_t switch_servo;
servo_t shake_servo;

// generic
bool alive = false; // switch on/off state
int32_t water_milliliters_last_refill = 0;

void init_drivers(void)
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

    // servo
    servo_setup(&shake_servo, &shake_servo_params);
    servo_setup(&switch_servo, &switch_servo_params);

    // water_pump
    water_pump_setup();

    // slide switch
    slide_switch_setup();

}

void refill_water(void)
{
    while(hx711_get_units(&water_hx_dev) < water_bowl_milliliters)
    {
        water_pump_on();
    }
    water_pump_off();
    water_milliliters_last_refill = hx711_get_units(&water_hx_dev);
}

void compute_water_dispenser(void)
{
    int32_t water_value = hx711_get_units(&water_hx_dev); /* Sample */
    printf("water value %"PRIu32"\n", water_value);

    if(!alive) {
        // init the water bowl
        refill_water();
        return;
    }

    if (water_value < water_milliliters_last_refill) {
        // check water consumption
        publish_message(water_milliliters_last_refill - water_value, mqtt_topic_water);
        refill_water();
        return;
    }
}

int main(void)
{
    init_drivers();
    
    xtimer_ticks32_t last_wakeup;
    bool is_last_wakeup = false;

    while (1) {

        if(slide_switch_read()) {
            compute_water_dispenser();
            //compute_food_dispencer();
            alive = true;
        } else {
            alive = false;
        }

        

        //uint32_t start_time = xtimer_now_usec();

        //servo_on(&switch_servo);
        //servo_on(&shake_servo);

        //uint32_t end_time = xtimer_now_usec();
        //printf("Microseconds difference: %d\n", end_time-start_time); 
        //start_time = end_time;


        /* Duty cycle */
        if (!is_last_wakeup) {
            /* set last_wakeup only the first time */
            is_last_wakeup = true;
            last_wakeup = xtimer_now();
        }
        xtimer_periodic_wakeup(&last_wakeup, US_PER_SEC * 30);
    }

    return 0;
}