#ifndef DRIVER_HX711_H
#define DRIVER_HX711_H

#include "periph/gpio.h"
#include <inttypes.h>

/**
 * @note Selectable gain and channel:
 *       Read the HX711 datasheet for more information
 */
typedef enum {
    CHANNEL_A_128 = 1,
    CHANNEL_B_32 = 2,
    CHANNEL_A_64 = 3
} hx711_gain_t;

/**
 * @brief   Configuration parameters of the HX711 device
 * @{
 */
typedef struct {
    gpio_t sck;         /**< SCK GPIO pin */
    gpio_t dout;        /**< DT or DOUT GPIO pin */
    hx711_gain_t gain;  /**< Gain and channel selection.
                         * Read HX711 documentation for more information. */
    uint8_t read_times; /**< How many times SAUL reads the value
                         *   and creates an average from the sum. */
    uint16_t divider;   /**< The devider in the formular:
                         *   ((SUM(RAW_VALUES) / TIMES) - OFFSET) / DIVIDER */
} hx711_params_t;

/**
 * @brief   Device descriptor for the HX711
 */
typedef struct {
    hx711_params_t params; /**< params for the hx711 */
    int32_t offset;        /**< Offset which will be set while taring */
} hx711_t;

/**
 * @brief Initializes the hx711 device, without taring it.
 * @param dev The device to initialize
 * @param params The parameters for the hx711 device
 */
void hx711_init(hx711_t *dev, const hx711_params_t *params);

/**
 * @brief Read the average of a configurable times
 *        of a cleared and divided value
 * @param dev The hx711 device
 * @return returns ((SUM(RAW_VALUES) / TIMES) - OFFSET) / DIVIDER
 */
int32_t hx711_get_units(hx711_t *dev);

/**
 * @brief Get the offset and set it to the device. Tare the scale.
 * @param dev The hx711 device
 */
void hx711_tare(hx711_t *dev);

/**
 * @brief Power a given hx711 down
 * @param dev The hx711 device
 */
void hx711_power_down(hx711_t *dev);

/**
 * @brief Power a given hx711 up
 * @param dev The hx711 device
 */
void hx711_power_up(hx711_t *dev);

/**
 * @brief Init and tare hx711
 * @param dev The device to initialize
 * @param params The parameters for the hx711 device
*/
void hx711_setup(hx711_t *dev, const hx711_params_t *params);

#endif
