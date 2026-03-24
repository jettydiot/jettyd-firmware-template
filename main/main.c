/**
 * jettyd firmware template — minimal main.c
 *
 * Edit sdkconfig.defaults to set your WiFi credentials and fleet token,
 * then: idf.py build flash monitor
 */

#include "jettyd.h"

void app_main(void)
{
    /* Minimal config — all values come from Kconfig (sdkconfig.defaults) */
    jettyd_config_t config = {
        .device_type          = CONFIG_JETTYD_DEVICE_TYPE,
        .firmware_version     = CONFIG_JETTYD_FIRMWARE_VERSION,
        .heartbeat_interval_sec = 60,
        .mqtt_keepalive       = 60,
        .mqtt_qos             = 1,
        .mqtt_buffer_on_disconnect = true,
        .mqtt_max_buffer_size = 16,
        .deep_sleep           = false,
        .has_battery          = false,
        .battery_adc_pin      = -1,
        .status_led_pin       = -1,
        .wake_on_pin          = -1,
    };

    /* Initialise: NVS, WiFi, MQTT, provisioning, shadow, drivers */
    ESP_ERROR_CHECK(jettyd_init(&config));

    /* Start: connect, provision if needed, begin telemetry loop (never returns) */
    ESP_ERROR_CHECK(jettyd_start());
}
