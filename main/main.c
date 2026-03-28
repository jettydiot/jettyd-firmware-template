/**
 * jettyd firmware template — main.c
 *
 * All device-specific values (heartbeat interval, report metrics, name,
 * version, MQTT settings) come from device.yaml via the auto-generated
 * device_config.h. Edit device.yaml, not this file.
 *
 * WiFi credentials and fleet token come from sdkconfig.defaults / Kconfig
 * because they contain secrets that should not be in source control.
 */

#include "jettyd.h"
#include "device_config.h"

void app_main(void)
{
    jettyd_config_t config = {
        /* Identity — from device.yaml */
        .device_type          = DEVICE_NAME,
        .firmware_version     = DEVICE_VERSION,

        /* Telemetry — from device.yaml defaults:                          */
        /* heartbeat_interval is in SECONDS. Fires immediately on connect, */
        /* then repeats at the configured interval.                        */
        /* DEVICE_REPORT_METRICS is either a NULL-terminated string array  */
        /* (specific metrics) or NULL (publish all available metrics).     */
        .heartbeat_interval_sec = DEVICE_HEARTBEAT_INTERVAL_SEC,
        .default_metrics        = DEVICE_REPORT_METRICS,

        /* MQTT — from device.yaml mqtt: block */
        .mqtt_keepalive            = DEVICE_MQTT_KEEPALIVE,
        .mqtt_qos                  = DEVICE_MQTT_QOS,
        .mqtt_buffer_on_disconnect = true,
        .mqtt_max_buffer_size      = 16,

        /* Power — defaults (override in device.yaml if needed) */
        .deep_sleep       = false,
        .has_battery      = false,
        .battery_adc_pin  = -1,
        .status_led_pin   = -1,
        .wake_on_pin      = -1,
    };

    ESP_ERROR_CHECK(jettyd_init(&config));
    ESP_ERROR_CHECK(jettyd_start());
}
