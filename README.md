# jettyd Firmware Template

A minimal ESP32 starter for connecting devices to [jettyd.com](https://jettyd.com).

## What is jettyd?

jettyd is IoT middleware that bridges your ESP32 devices to AI agents, dashboards, and automation rules. Your device sends telemetry → jettyd routes it → your agents and dashboards respond.

## SDK

This template uses the [jettyd-firmware SDK](https://github.com/jettydiot/jettyd-firmware) — the open-source ESP32 device SDK for jettyd. The SDK handles WiFi, MQTT, provisioning, telemetry, device shadow, OTA, and the JettyScript VM.

## Quick start

1. **Clone this repo**
   ```bash
   git clone https://github.com/jettydiot/jettyd-firmware-template
   cd jettyd-firmware-template
   ```

2. **Configure your device** — edit `device.yaml`:
   ```yaml
   name: "my-device-v1"
   target: "esp32s3"
   drivers:
     - name: dht22
       instance: "air"
       config:
         pin: 4
   ```

3. **Build**
   ```bash
   python tools/build.py --device my-device-v1 --target esp32s3
   ```

4. **Flash + provision**
   ```bash
   python tools/flash.py \
     --device my-device-v1 \
     --port /dev/ttyUSB0 \
     --fleet-token ft_live_xxxxx
   ```

## Prerequisites

- [ESP-IDF v5.5+](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/)
- Python 3.8+
- A jettyd account — [sign up at jettyd.com](https://jettyd.com)

## Documentation

Full documentation at [docs.jettyd.com](https://docs.jettyd.com)

## License

MIT
