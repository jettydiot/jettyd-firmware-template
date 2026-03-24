/**
 * Jettyd Firmware Template
 * 
 * This is the minimal main.c for a jettyd-connected device.
 * The jettyd SDK handles WiFi, MQTT, provisioning, OTA, and telemetry.
 * Your only job: call jettyd_init() and jettyd_start().
 * 
 * See device.yaml to configure which sensors/actuators to include.
 * See docs.jettyd.com for the full SDK reference.
 */

#include "jettyd.h"

void app_main(void) {
    // Initialize the jettyd SDK
    // Reads config from NVS, connects to WiFi, provisions with fleet token
    jettyd_init();

    // Start the main loop
    // Handles: MQTT connection, telemetry publishing, command handling,
    //          JettyScript rule evaluation, OTA updates
    jettyd_start();

    // jettyd_start() does not return — it runs forever
}
