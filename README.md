# jettyd firmware template

Minimal ESP32-S3/C3/C6 starter for [jettyd](https://jettyd.com) — the IoT middleware for AI agents.

## Quick start

👉 **[Full QuickStart guide →](https://docs.jettyd.com/quickstart)**

### Prerequisites

1. **[ESP-IDF v5.x](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)** installed and on your `PATH` (`idf.py` must work in your shell).
2. An **ESP32-S3 / C3 / C6** board (original ESP32 not supported) and a USB cable.
3. A **fleet token** from [jettyd.com](https://jettyd.com).

### Steps

```bash
# 1. Clone the template
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device

# 2. Fetch the firmware SDK (clones jettyd-sdk/ as a sibling)
make setup

# 3. Open device.yaml in your editor and set wifi_ssid, wifi_password,
#    and fleet_token — see snippet below.

# 4. Build, flash, and watch logs (target is auto-detected from device.yaml)
idf.py build flash monitor
```

Minimal `device.yaml`:

```yaml
jettyd:
  fleet_token: "ft_your_token_here"
  mqtt_uri: "mqtt://mqtt.jettyd.com:1883"

wifi:
  ssid: "YourNetworkName"
  password: "YourNetworkPassword"

name: "my-device"
version: "1.0.0"
target: "esp32s3"   # esp32s3 | esp32c3 | esp32c6
```

`build.py` syncs these fields into `sdkconfig.defaults` automatically — you don't edit `sdkconfig.defaults` by hand.

## What's in the box

```
my-device/
├── device.yaml          ← declare your drivers here (LED, button, sensors)
├── build.py             ← auto-generates C code from device.yaml
├── main/main.c          ← entry point (usually no changes needed)
├── jettyd-sdk/          ← firmware SDK (submodule — don't edit)
└── sdkconfig.defaults   ← WiFi credentials + fleet token
```

## Supported hardware

| Chip | Status |
|------|--------|
| ESP32-S3 | ✅ Supported |
| ESP32-C3 | ✅ Supported |
| ESP32-C6 | ✅ Supported |

Original ESP32 is **not supported**.

## Documentation

- **[QuickStart](https://docs.jettyd.com/quickstart)** — connect your first device in 5 minutes
- **[API Reference](https://docs.jettyd.com/api)** — REST API for device management
- **[Firmware SDK](https://github.com/jettydiot/jettyd-firmware)** — driver API, JettyScript VM
- **[MCP Server](https://www.npmjs.com/package/@jettyd/mcp)** — AI agent integration

## License

MIT — see [LICENSE](LICENSE).
