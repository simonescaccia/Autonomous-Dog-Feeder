#include "slide_switch.h"
#include "app_params.h"

void slide_switch_setup(void)
{
    gpio_init(SLIDE_PIN, GPIO_IN);
}

bool slide_switch_read(void)
{
    return gpio_read(SLIDE_PIN);
} 