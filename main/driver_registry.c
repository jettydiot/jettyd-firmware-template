/**
 * driver_registry.c — register your project's drivers here.
 *
 * This file lives in YOUR project (main/), not in the jettyd-sdk submodule.
 * Edit freely. Do not edit jettyd-sdk/jettyd/src/driver_registry.c.
 *
 * This file is auto-generated from device.yaml when build.py is present.
 * Manual edits will be overwritten on next build if device.yaml is used.
 */

#include "jettyd_driver.h"
/* Add your driver headers here, e.g.:
 * #include "dht22.h"
 * #include "led.h"
 */

void jettyd_register_drivers(void)
{
    /* Register your drivers here. Example:
     *
     *   dht22_config_t air_cfg = { .pin = 4 };
     *   dht22_register("air", &air_cfg);
     *
     *   led_config_t led_cfg = { .pin = 8, .active_high = true };
     *   led_register("status", &led_cfg);
     */
}
