/**
 * driver_registry.c — AUTO-GENERATED from device.yaml
 * Do not edit manually. Edit device.yaml and rebuild.
 */

#include "jettyd_driver.h"
#include "dht22.h"

void jettyd_register_drivers(void)
{
    dht22_config_t air_cfg = { .pin = 4 };
    dht22_register("air", &air_cfg);

}
