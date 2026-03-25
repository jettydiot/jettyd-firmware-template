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

# 3. Edit sdkconfig.defaults — set your credentials:
#    CONFIG_JETTYD_FLEET_TOKEN="ft_xxxxxxxxxxxxxxxxxxxx"
#    CONFIG_JETTYD_WIFI_SSID="YourNetworkName"
#    CONFIG_JETTYD_WIFI_PASSWORD="YourNetworkPassword"

# 4. Build and flash
rm -f sdkconfig   # always start clean after editing defaults
idf.py build flash monitor
```

On first boot the device connects to WiFi, provisions itself with jettyd, and enters its main loop. You'll see:

```
I (13600) jettyd:   Jettyd running
I (13600) jettyd:   Drivers: 0
I (13600) jettyd:   Rules: 0
```

---

## Adding a sensor

### Step 1 — Wire the sensor

Connect your DHT22 data pin to a GPIO (e.g. GPIO 4).

### Step 2 — Declare the driver in `device.yaml`

Open `device.yaml` in your project root and add the driver under `drivers:`:

```yaml
drivers:
  - name: dht22
    instance: "air"
    config:
      pin: 4
```

That's it. The next `idf.py build` auto-generates `main/driver_registry.c` from `device.yaml`.
The driver will publish `air.temperature` and `air.humidity` on every heartbeat.

> **Note:** Never edit `jettyd-sdk/` files directly — it's a git submodule.
> Your driver registration lives in `main/driver_registry.c` (auto-generated)
> or you can edit it manually if you prefer not to use `device.yaml`.


---

## Controlling an LED

### Step 1 — Wire the LED

Connect an LED (with a 330Ω resistor in series) from GPIO 8 to GND.

### Step 2 — Register the LED driver

In `device.yaml`:

```yaml
drivers:
  - name: led
    instance: "status"
    config:
      pin: 8
      active_high: true
```

### Step 3 — Control it via jettyd

```bash
# Turn on
curl -X POST https://api.jettyd.com/v1/devices/DEVICE_ID/commands \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{"action": "led.on"}'

# Blink 3 times
curl -X POST https://api.jettyd.com/v1/devices/DEVICE_ID/commands \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{"action": "led.blink", "params": {"interval_ms": 300, "count": 3}}'

# Turn off
curl -X POST https://api.jettyd.com/v1/devices/DEVICE_ID/commands \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{"action": "led.off"}'
```

Or ask your OpenClaw agent:
> *"Blink the status LED 3 times."*

---

## Using a button

### Step 1 — Wire the button

Connect a momentary push button between GPIO 9 and GND. The driver uses the internal pull-up.

### Step 2 — Register the button driver

```c
#include "button.h"

void jettyd_register_drivers(void)
{
    button_config_t btn = { .pin = 9, .active_low = true, .debounce_ms = 50 };
    button_register("main", &btn);
}
```

### Step 3 — Read button events

The button publishes two metrics:
- `main.press` — 1 while held, 0 when released
- `main.press_count` — total presses since boot

```bash
curl https://api.jettyd.com/v1/devices/DEVICE_ID/shadow \
  -H "Authorization: Bearer YOUR_API_KEY"
# → {"reported": {"main.press": 0, "main.press_count": 7}}
```

Or use webhooks to trigger an automation whenever the button is pressed.

---

### Available drivers

| Driver | Sensors | Include | Config |
|--------|---------|---------|--------|
| `dht22` | Temperature, humidity | `dht22.h` | `{ .pin = N }` |
| `ds18b20` | Temperature (1-Wire) | `ds18b20.h` | `{ .pin = N }` |
| `bme280` | Temperature, humidity, pressure | `bme280.h` | `{ .i2c_port = 0, .sda = 21, .scl = 22, .addr = 0x76 }` |
| `hcsr04` | Distance (ultrasonic) | `hcsr04.h` | `{ .trig_pin = N, .echo_pin = M }` |
| `ina219` | Current, voltage, power | `ina219.h` | `{ .i2c_port = 0, .sda = 21, .scl = 22 }` |
| `soil_moisture` | Soil moisture (ADC) | `soil_moisture.h` | `{ .adc_pin = N }` |
| `relay` | Relay output | `relay.h` | `{ .pin = N, .active_high = true }` |
| `pwm_output` | PWM actuator | `pwm_output.h` | `{ .pin = N, .freq_hz = 1000 }` |
| `led` | LED output (on/off/blink) | `led.h` | `{ .pin = N, .active_high = true }` |
| `button` | Button/switch input | `button.h` | `{ .pin = N, .active_low = true, .debounce_ms = 50 }` |

---


---

## JettyScript — rules that run on the device

JettyScript is a lightweight rules engine that runs on the ESP32 itself — no cloud round-trip needed. Rules evaluate sensor readings locally and fire alerts when thresholds are crossed.

### Example: temperature alerts

This config fires a warning when temperature exceeds 28°C, and a critical alert below 5°C:

```json
{
  "rules": [
    {
      "id": "temp-too-hot",
      "when": {
        "type": "threshold",
        "sensor": "air.temperature",
        "op": ">",
        "value": 28,
        "debounce": 30
      },
      "then": [
        {
          "action": "publish_alert",
          "params": {
            "message": "Temperature too high: {{air.temperature}}°C",
            "severity": "warning"
          }
        }
      ]
    },
    {
      "id": "temp-too-cold",
      "when": {
        "type": "threshold",
        "sensor": "air.temperature",
        "op": "<",
        "value": 5,
        "debounce": 30
      },
      "then": [
        {
          "action": "publish_alert",
          "params": {
            "message": "Temperature too low: {{air.temperature}}°C",
            "severity": "critical"
          }
        }
      ]
    }
  ]
}
```

**`debounce: 30`** — the condition must hold for 30 seconds before firing. Prevents alert spam from noisy sensors.

### Push the config to your device

```bash
curl -X PUT https://api.jettyd.com/v1/devices/DEVICE_ID/config \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d @rules.json
```

The platform pushes the config to the device over MQTT. The device validates it, applies it, and ACKs — no reflashing needed.

### Forward alerts to a webhook

When a rule fires, it publishes an alert event on the platform. Subscribe a webhook to receive it:

```bash
curl -X POST https://api.jettyd.com/v1/webhooks \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Temperature alerts → Slack",
    "url": "https://hooks.slack.com/services/YOUR/WEBHOOK/URL",
    "events": ["device.alert.warning", "device.alert.critical"],
    "secret": "your-signing-secret"
  }'
```

Every alert the device fires hits your webhook within milliseconds. The payload is HMAC-signed so you can verify it came from jettyd.

### Supported rule conditions

| Type | Description | Example |
|------|-------------|---------|
| `threshold` | Single value above/below a limit | `temperature > 28` |
| `range` | Value leaves a safe range | `humidity` outside 30–70% |
| `compound` | AND/OR of multiple conditions | temp high AND humidity high |
| `time_window` | Only active during certain hours | alert only during work hours |
| `cron` | Fire on a schedule | report every hour |

---
## Connecting to OpenClaw

Once your device is provisioned, connect it to your OpenClaw agent in seconds.

### 1. Install the jettyd skill (coming soon to ClawHub)

For now, query the API directly from any OpenClaw skill or tool:

```bash
# List your devices
curl https://api.jettyd.com/v1/devices \
  -H "Authorization: Bearer YOUR_API_KEY"

# Read the latest sensor readings (device shadow)
curl https://api.jettyd.com/v1/devices/DEVICE_ID/shadow \
  -H "Authorization: Bearer YOUR_API_KEY"

# Send a command (e.g. toggle a relay)
curl -X POST https://api.jettyd.com/v1/devices/DEVICE_ID/commands \
  -H "Authorization: Bearer YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{"action": "relay.on", "params": {"duration": 5000}}'
```

### 2. Ask your agent about your devices

Once the OpenClaw jettyd skill is available, you'll be able to do things like:

```
You: What's the temperature in the greenhouse?
Agent: The air sensor reads 24.3°C and 67% humidity. Last updated 2 minutes ago.

You: Turn on the irrigation relay for 30 seconds.
Agent: Done — relay activated, will auto-off in 30 seconds.
```

### 3. Your device's API keys

| Key | Value |
|-----|-------|
| API key | `tk_jettyd_tom_dev_key_2026` |
| Tenant ID | `2bf59dbe-e121-4597-84f9-1f4ded2437fa` |
| Device ID | `930c4077-fbe7-48f2-824c-c9e806175f58` |

---

## Configuration (sdkconfig.defaults)

| Key | Description |
|-----|-------------|
| `CONFIG_JETTYD_FLEET_TOKEN` | Fleet token from api.jettyd.com/v1/fleet-tokens |
| `CONFIG_JETTYD_WIFI_SSID` | WiFi network name |
| `CONFIG_JETTYD_WIFI_PASSWORD` | WiFi password |
| `CONFIG_JETTYD_MQTT_URI` | MQTT broker (default: `mqtt://mqtt.jettyd.com:1883`) |
| `CONFIG_JETTYD_FIRMWARE_VERSION` | Version string reported to the platform |
| `CONFIG_JETTYD_DEVICE_TYPE` | Device type slug from your jettyd account |

> **Always delete `sdkconfig` before rebuilding after editing `sdkconfig.defaults`:**
> ```bash
> rm sdkconfig && idf.py build
> ```

## SDK

This template uses the [jettyd-firmware SDK](https://github.com/jettydiot/jettyd-firmware) (MIT). The SDK handles WiFi, MQTT, fleet provisioning, telemetry, device shadow, OTA, and the JettyScript rules VM.

## Supported hardware

| Chip | Status |
|------|--------|
| ESP32-C3 | ✅ Tested |
| ESP32-S3 | ✅ Supported |
| ESP32-C6 | ✅ Supported |
| ESP32 (classic) | ⚠️ Untested |

## Licence

MIT
