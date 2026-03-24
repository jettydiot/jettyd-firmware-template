# jettyd firmware template

Minimal ESP32 starter for [jettyd.com](https://jettyd.com) — the IoT middleware layer for AI agents.

## Prerequisites

- [ESP-IDF 5.x](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) installed and sourced
- An ESP32-S3, C3, or C6 board
- A jettyd account → [jettyd.com](https://jettyd.com) (free tier: 5 devices)

## Quick start

```bash
# 1. Clone this template
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device

# 2. Clone the jettyd SDK
make setup

# 3. Edit device.yaml — set your WiFi credentials and fleet token
#    Get your fleet token from api.jettyd.com/v1/fleet-tokens
nano device.yaml

# 4. Flash the config to NVS (WiFi + fleet token)
python3 tools/flash_config.py device.yaml  # coming soon — see below

# 5. Build and flash
idf.py build flash monitor
```

## device.yaml

```yaml
# ── Required ──────────────────────────────────────────────────────────────────
jettyd:
  fleet_token: "ft_xxxxxxxxxxxxxxxxxxxx"   # from api.jettyd.com/v1/fleet-tokens
  mqtt_uri: "mqtt://mqtt.jettyd.com:1883"

wifi:
  ssid: "YourNetworkName"
  password: "YourNetworkPassword"

# ── Device identity ───────────────────────────────────────────────────────────
name: "my-sensor-v1"
version: "1.0.0"
target: "esp32s3"

# ── Drivers ───────────────────────────────────────────────────────────────────
drivers:
  - name: dht22
    instance: "air"
    config:
      pin: 4
```

## How it works

1. On first boot the device connects to WiFi and sends the fleet token to jettyd
2. jettyd returns a `device_key` — stored in NVS, used for all future auth
3. Telemetry flows via MQTT → jettyd stores it, fires webhooks, and exposes it over the REST API
4. Your AI agent (OpenClaw, etc.) calls `GET /v1/devices/:id/shadow` and `POST /v1/devices/:id/command`

## Setting WiFi + fleet token (NVS flash)

The config needs to land in NVS before the device can connect. Until the `flash_config.py` tool is ready, set them via `idf.py menuconfig`:

```
Component config → jettyd → Fleet token
Component config → jettyd → WiFi SSID / Password
```

Or bake them as compile-time defaults in `sdkconfig.defaults`:

```
CONFIG_JETTYD_FLEET_TOKEN="ft_xxxxxxxxxxxxxxxxxxxx"
CONFIG_JETTYD_MQTT_URI="mqtt://mqtt.jettyd.com:1883"
CONFIG_JETTYD_WIFI_SSID="YourNetworkName"
CONFIG_JETTYD_WIFI_PASSWORD="YourNetworkPassword"
```

## SDK

This template uses the [jettyd-firmware SDK](https://github.com/jettydiot/jettyd-firmware) (MIT). The SDK handles WiFi, MQTT, fleet provisioning, telemetry, device shadow, OTA, and the JettyScript rules VM.

## Licence

MIT
