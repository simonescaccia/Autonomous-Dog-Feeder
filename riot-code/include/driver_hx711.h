#include "periph/gpio.h"
#include <inttypes.h>

/**
 * @brief   Configuration parameters of the HX711 device
 * @{
 */
typedef struct {
    gpio_t sck;         /**< SCK GPIO pin */
    gpio_t dout;        /**< DT or DOUT GPIO pin */
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
 * @param times The amount of repeated measurements.
 * @return returns ((SUM(RAW_VALUES) / TIMES) - OFFSET) / DIVIDER
 */
int32_t hx711_get_units(hx711_t *dev, int8_t times);