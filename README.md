# jettyd firmware template

Minimal ESP32 starter for [jettyd.com](https://jettyd.com) — the IoT middleware layer for AI agents.

## Prerequisites

- [ESP-IDF 5.x](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) installed and sourced
- An ESP32-S3, C3, or C6 board
- A jettyd account — get your fleet token from [jettyd.com](https://jettyd.com)

## Quick start

```bash
# 1. Clone this template
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device

# 2. Fetch the jettyd SDK
make setup

# 3. Configure — edit sdkconfig.defaults
#    Set your WiFi credentials and fleet token:
#
#    CONFIG_JETTYD_FLEET_TOKEN="ft_xxxxxxxxxxxxxxxxxxxx"
#    CONFIG_JETTYD_WIFI_SSID="YourNetworkName"
#    CONFIG_JETTYD_WIFI_PASSWORD="YourNetworkPassword"

# 4. Build and flash
idf.py build flash monitor
```

On first boot the device:
1. Reads config from `sdkconfig.defaults` → writes to NVS
2. Connects to WiFi
3. Sends the fleet token to jettyd and receives a device key
4. Starts publishing telemetry via MQTT

After ~10 seconds it appears in your device list at `api.jettyd.com/v1/devices`.

## Configuration (sdkconfig.defaults)

All config lives in `sdkconfig.defaults`. Edit before building:

| Key | Description |
|-----|-------------|
| `CONFIG_JETTYD_FLEET_TOKEN` | Fleet token from api.jettyd.com/v1/fleet-tokens |
| `CONFIG_JETTYD_WIFI_SSID` | Your WiFi network name |
| `CONFIG_JETTYD_WIFI_PASSWORD` | Your WiFi password |
| `CONFIG_JETTYD_MQTT_URI` | MQTT broker (default: `mqtt://mqtt.jettyd.com:1883`) |
| `CONFIG_JETTYD_FIRMWARE_VERSION` | Version string reported to the platform |
| `CONFIG_JETTYD_DEVICE_TYPE` | Device type slug from your jettyd account |

## SDK

This template uses the [jettyd-firmware SDK](https://github.com/jettydiot/jettyd-firmware) (MIT). The SDK handles WiFi, MQTT, fleet provisioning, telemetry, device shadow, OTA, and the JettyScript rules VM.

## Supported hardware

| Chip | Status |
|------|--------|
| ESP32-S3 | ✅ Primary target |
| ESP32-C3 | ✅ Supported |
| ESP32-C6 | ✅ Supported |
| ESP32 (classic) | ⚠️ Untested |

## Licence

MIT
