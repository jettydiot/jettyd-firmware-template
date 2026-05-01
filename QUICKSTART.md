# QuickStart — connect your first device in 5 minutes

> This file is the **single source of truth** for the jettyd quickstart.
> [docs.jettyd.com/quickstart](https://docs.jettyd.com/quickstart) and the
> [app.jettyd.com](https://app.jettyd.com) Get Started page both render
> from this file. Edit it here, and both downstream sites update on the
> next deploy.

Connect your first ESP32 device to jettyd and see it appear on your dashboard.

## Prerequisites

| What | Why |
|------|-----|
| **[ESP-IDF v5.x](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)** on your `PATH` | Build toolchain for ESP32 — `idf.py` must work in your shell |
| **ESP32-S3**, **ESP32-C3**, or **ESP32-C6** dev board | Original ESP32 not supported |
| USB cable | Flash & serial monitor |
| WiFi network | Device joins your local network |
| **Fleet token** from [jettyd.com](https://jettyd.com) | Provisions the device on the platform |

> 💡 **No jettyd account yet?** Sign up at [jettyd.com](https://jettyd.com) to get your fleet token. During early access, email [hello@jettyd.com](mailto:hello@jettyd.com).

## Steps

### 1. Clone the template

```bash
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device
```

### 2. Fetch the firmware SDK

```bash
make setup
```

This clones `jettyd-firmware` into `jettyd-sdk/` next to the template.

> ⚠️ Skip this step and the next `idf.py` invocation will fail with **"jettyd SDK not found. Run: make setup"**.

After `make setup`, your project looks like:

```
my-device/
├── device.yaml          ← your device config (you edit this)
├── build.py             ← YAML → C codegen (you don't run this directly)
├── main/main.c          ← entry point (usually no changes needed)
├── jettyd-sdk/          ← firmware SDK (cloned by `make setup`, don't edit)
└── sdkconfig.defaults   ← auto-synced from device.yaml (don't edit)
```

### 3. Configure your device

Open `device.yaml` in your editor and set `fleet_token`, `wifi.ssid`, and `wifi.password`. The same file declares your sensors and actuators:

```yaml
# device.yaml
jettyd:
  fleet_token: "ft_your_token_here"
  mqtt_uri: "mqtt://mqtt.jettyd.com:1883"

wifi:
  ssid: "YourNetworkName"
  password: "YourNetworkPassword"

name: "my-device"
version: "1.0.0"
target: "esp32s3"       # esp32s3 | esp32c3 | esp32c6

drivers:
  - name: led
    instance: "status"
    config:
      pin: 8
      active_high: true

  - name: button
    instance: "btn"
    config:
      pin: 0
      active_low: true
```

`build.py` syncs `fleet_token`, `wifi.*`, `version`, and `target` into `sdkconfig.defaults` automatically — you don't edit `sdkconfig.defaults` by hand.

> ⚠️ **Important:** after editing `device.yaml`, delete `sdkconfig` before rebuilding — ESP-IDF caches config and won't otherwise pick up your changes:
>
> ```bash
> rm -f sdkconfig
> ```

### 4. Build, flash, and watch logs

```bash
idf.py build flash monitor
```

The build system auto-generates `main/driver_registry.c` from your `device.yaml` and auto-detects the IDF target. No manual C edits needed.

On first boot you'll see:

```
I (3200) jettyd_wifi: Connected — IP: 192.168.1.42
I (4800) jettyd_prov: Provisioning... fleet_token=ft_...
I (5200) jettyd_prov: Provisioned! device_key=dk_abcdef123456
I (5600) jettyd_mqtt: Connected to mqtt.jettyd.com
I (5800) jettyd:   Jettyd running
I (5800) jettyd:   Drivers: 1
I (5800) jettyd:   Rules: 0
```

> ✅ **Your device is now online.** It connected to WiFi, provisioned itself with jettyd, and entered its main loop. Telemetry publishes on every heartbeat.

### 5. See it on the dashboard

Open [app.jettyd.com](https://app.jettyd.com) and log in. Your device appears in the device list with a green **● Online** indicator.

From the device detail page you can:

- View live telemetry (temperature, humidity, etc.)
- Inspect the device shadow (reported vs. desired state)
- Send commands (`reboot`, `set_interval`, etc.)
- See command history and delivery status

### 6. Control it with AI

jettyd exposes a REST API and an [MCP server](https://www.npmjs.com/package/@jettyd/mcp) that any AI agent can use:

```bash
# Install the MCP server
npx @jettyd/mcp

# Or use the REST API directly
curl -H "Authorization: Bearer YOUR_API_KEY" \
  https://api.jettyd.com/v1/devices
```

## Adding more drivers

Just add entries to `device.yaml`:

```yaml
drivers:
  - name: dht22
    instance: "air"
    config:
      pin: 4

  - name: led
    instance: "status"
    config:
      pin: 8
      active_high: true
```

Run `idf.py build flash` — the codegen handles the rest.

### Available drivers

| Driver | Type | Metrics / Actions |
|--------|------|-------------------|
| `dht22` | Sensor | temperature, humidity |
| `bme280` | Sensor | temperature, humidity, pressure |
| `ds18b20` | Sensor | temperature |
| `soil_moisture` | Sensor | moisture (0–100%) |
| `hcsr04` | Sensor | distance_cm |
| `ina219` | Sensor | voltage, current, power |
| `led` | Actuator | on, off, blink |
| `relay` | Actuator | on, off |
| `button` | Input | press, long_press, double_press |
| `pwm_output` | Actuator | duty cycle (0–100%) |

## Troubleshooting

### Device stuck on "Provisioning…"

Check that your fleet token is correct and the device can reach `mqtt.jettyd.com:1883`. Firewalls sometimes block MQTT.

### WiFi won't connect

Make sure you ran `rm -f sdkconfig` after editing `device.yaml`. ESP-IDF caches config and bakes WiFi credentials in at configure time.

### Build fails: "jettyd SDK not found"

Run `make setup` to clone the SDK, or manually:

```bash
git clone https://github.com/jettydiot/jettyd-firmware.git jettyd-sdk
```

### Stack overflow / crash on boot

The default stack sizes are tuned for most projects. If you're using many drivers, increase in `sdkconfig.defaults`:

```
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE=6144
```

## Next steps

- **[API Reference](https://docs.jettyd.com)** — full REST API docs with interactive examples
- **[Firmware SDK](https://github.com/jettydiot/jettyd-firmware)** — driver API, JettyScript VM, advanced configuration
- **[MCP Server](https://www.npmjs.com/package/@jettyd/mcp)** — connect your AI agent to jettyd devices
