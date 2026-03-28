/**
 * driver_registry.c — AUTO-GENERATED from device.yaml
 * Do not edit manually. Edit device.yaml and rebuild.
 */

#include "jettyd_driver.h"
#include "led.h"
#include "button.h"

void jettyd_register_drivers(void)
{
    led_config_t status_cfg = { .pin = 8, .active_high = true };
    led_register("status", &status_cfg);

    button_config_t btn_cfg = { .pin = 0, .active_low = true };
    button_register("btn", &btn_cfg);

}
