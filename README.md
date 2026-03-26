# jettyd firmware template

Minimal ESP32-S3/C3/C6 starter for [jettyd](https://jettyd.com) — the IoT middleware for AI agents.

## Quick start

👉 **[Full QuickStart guide →](https://docs.jettyd.com/quickstart)**

```bash
git clone https://github.com/jettydiot/jettyd-firmware-template my-device
cd my-device && make setup
# Edit device.yaml with your WiFi + fleet token
idf.py build flash monitor
```

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
