# QuickStart — jettyd firmware template

Connect an ESP32 device to [jettyd.com](https://jettyd.com) in under 10 minutes.

---

## Option A — Browser flasher (recommended, no toolchain needed)

Flash jettyd firmware onto an **ESP32-C3** directly from your browser — no ESP-IDF install, no command line.

👉 **[flash.jettyd.com](https://flash.jettyd.com)** — one click, then follow the Wi-Fi provisioning prompts.

Step-by-step instructions: **[FLASH.md](./FLASH.md)**

> Use Option B below if you need a different chip target (ESP32-S3, ESP32-C6), custom drivers, or a full developer environment.

---

## Option B — ESP-IDF (full developer workflow)

### Prerequisites

| Requirement | Notes |
|---|---|
| ESP-IDF v5.2+ | [Installation guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) |
| Python 3.9+ | For `build.py` codegen |
| Git | To clone this template |
| jettyd account | [Sign up free →](https://app.jettyd.com/signup) |

Supported targets: **ESP32-S3**, **ESP32-C3**, **ESP32-C6**

---

### 1. Clone the template

```bash
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device
```

---

### 2. Get your fleet token

1. Log in to [app.jettyd.com](https://app.jettyd.com)
2. Navigate to **Fleet → Tokens → New token**
3. Copy the `ft_…` token

---

### 3. Configure your device

Open `device.yaml` and fill in the required fields:

```yaml
jettyd:
  fleet_token: "ft_YOUR_TOKEN_HERE"   # from step 2
  mqtt_uri: "mqtt://mqtt.jettyd.com:1883"

wifi:
  ssid: "YourNetworkName"
  password: "YourNetworkPassword"

name: "my-device"
target: "esp32s3"   # esp32s3 | esp32c3 | esp32c6
```

> ⚠️ Never commit real tokens. Add `device.yaml` to `.gitignore` or use environment variable substitution before committing.

---

### 4. Set up the SDK

```bash
make setup
```

This clones `jettydiot/jettyd-firmware` into `jettyd-sdk/` — the SDK that ships your device's telemetry and command handling to jettyd.

---

### 5. Build and flash

```bash
idf.py build flash monitor
```

Or use the Makefile shortcuts:

```bash
make build           # compile only
make flash           # compile + flash
make flash-monitor   # compile + flash + open serial monitor
```

On first run, `build.py` auto-generates:
- `main/device_config.h` — typed device configuration
- `main/driver_registry.c` — driver wiring
- `main/driver_requires.cmake` — build dependencies

---

### 6. Verify the connection

Once the device boots you should see in the serial monitor:

```
I (1234) jettyd: Connected to WiFi
I (1456) jettyd: MQTT connected → mqtt.jettyd.com
I (1460) jettyd: Device registered: my-device
I (1462) jettyd: Telemetry heartbeat sent
```

Head to [app.jettyd.com](https://app.jettyd.com) → **Devices** — your device should appear online within a few seconds.

---

### 7. Add drivers

Edit the `drivers:` section in `device.yaml` to map your hardware:

```yaml
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

Re-run `idf.py build flash` after every `device.yaml` change — codegen runs automatically.

---

### 8. Run checks before committing

```bash
make check
```

Runs codegen validation, format checks, and the SDK unit test suite. All three must pass before pushing.

---

### Telemetry defaults

```yaml
defaults:
  heartbeat_interval: 60    # seconds between telemetry pushes
  report_metrics:
    - "system.rssi"
    # - "system.chip_temp"
    # - "system.free_heap"
```

---

### Troubleshooting

**Device doesn't appear online**
- Verify `fleet_token` is correct and hasn't expired
- Check serial output for MQTT errors
- Ensure port 1883 is not blocked by your network

**Build fails with "sdkconfig mismatch"**
- `device.yaml` changed since last build: `rm sdkconfig && idf.py build`
- Or just run `make build` — the Makefile handles this automatically

**`make setup` fails**
- Check you have Git and a working internet connection
- If `jettyd-sdk/` already exists, remove it and re-run: `rm -rf jettyd-sdk && make setup`

---

### Next steps

- 📖 [Full SDK documentation](https://docs.jettyd.com)
- 🤖 [Connect an AI agent to your fleet](https://docs.jettyd.com/agents)
- 💬 [Community Discord](https://discord.gg/jettyd)
- 🐛 [Report an issue](https://github.com/jettydiot/jettyd-firmware-template/issues)
