# Flash jettyd Firmware via Browser

Flash your ESP32-C3 with jettyd firmware in under 2 minutes — no toolchain, no drivers, no command line.

> **Recommended for first-time setup.** For advanced customization (custom drivers, device configuration), see [QUICKSTART.md](./QUICKSTART.md).

---

## Requirements

| Requirement | Notes |
|---|---|
| **Google Chrome** or **Microsoft Edge** | Firefox and Safari do not support the WebSerial API required for flashing |
| **ESP32-C3** board | Other chips (S3, C6) are supported via the ESP-IDF path; see QUICKSTART.md |
| **Data-capable USB cable** | Charge-only cables will not enumerate a serial port |
| **jettyd account** | [Sign up free →](https://app.jettyd.com/signup) |

---

## Step 1 — Open the web flasher

Navigate to **[flash.jettyd.com](https://flash.jettyd.com)** in Chrome or Edge.

> If you are reading this offline, you can also open `flash.html` from the repo root after serving it over HTTPS (GitHub Pages or `npx serve`).

---

## Step 2 — Connect your device

1. Plug your ESP32-C3 into your computer with a USB cable.
2. On the flash page, click **Connect & Flash**.
3. A browser dialog appears listing available serial ports — select the port for your ESP32-C3.
   - On macOS: typically `/dev/cu.usbmodem…` or `/dev/cu.SLAB_…`
   - On Windows: typically `COM3`, `COM4`, etc.
   - On Linux: typically `/dev/ttyUSB0` or `/dev/ttyACM0`

---

## Step 3 — Flash

4. Click **Install** in the ESP Web Tools dialog to confirm.
5. The flasher erases the chip and writes the jettyd firmware (~2–4 minutes on first install, ~1 minute thereafter).
6. A progress bar shows flash progress. Do not unplug the device during this step.

---

## Step 4 — Wi-Fi provisioning

After flashing completes, the device reboots and the Improv Wi-Fi provisioning dialog appears automatically:

7. Enter your **Wi-Fi network name** (SSID) and **password**.
8. Click **Connect** — the device joins the network and calls home to jettyd.

> If the provisioning dialog does not appear, open a serial monitor at 115200 baud and look for `I (…) jettyd: Waiting for Improv provisioning`.

---

## Step 5 — Enter your fleet token

9. When prompted (or via the serial monitor), enter your **fleet token** (`ft_…`).
   - Get a token at [app.jettyd.com](https://app.jettyd.com) → **Fleet → Tokens → New token**.
10. The device registers itself and begins sending telemetry.

---

## Step 6 — Verify the connection

Head to [app.jettyd.com](https://app.jettyd.com) → **Devices** — your device should appear online within a few seconds.

You can also confirm via serial monitor:

```
I (1234) jettyd: Connected to WiFi
I (1456) jettyd: MQTT connected → mqtt.jettyd.com
I (1460) jettyd: Device registered: my-device
I (1462) jettyd: Telemetry heartbeat sent
```

---

## Troubleshooting

**"No port selected" / port not listed**
- Try a different USB cable — many cables are charge-only.
- On macOS, you may need to install the [CP2102 driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) for some boards.
- On Linux, add your user to the `dialout` group: `sudo usermod -aG dialout $USER` (then log out and back in).

**Flash fails partway through**
- Click the retry button in the flasher dialog.
- If it fails repeatedly, try holding the BOOT button on the ESP32-C3 while clicking flash.

**Device does not appear online after provisioning**
- Verify the fleet token is correct and hasn't expired.
- Check serial output for MQTT errors.
- Ensure port 1883 is not blocked by your network firewall.

**Need to re-flash**
- Click **Connect & Flash** again — the flasher overwrites the existing firmware.

---

## Next steps

Once your device is online, see [QUICKSTART.md](./QUICKSTART.md) to add drivers, configure telemetry, and build custom firmware.

- [Full SDK documentation →](https://docs.jettyd.com)
- [Connect an AI agent to your fleet →](https://docs.jettyd.com/agents)
- [Community Discord →](https://discord.gg/jettyd)
