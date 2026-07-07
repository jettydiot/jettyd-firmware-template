# jettyd-firmware-template — Agent Guide

Reference ESP-IDF project for jettyd devices (target: **esp32c3**, matches the
connected test board). Device behavior is configured in `device.yaml`;
`build.py` generates `main/driver_registry.c` from it — edit the YAML, not the
generated file.

## Dev commands

- `. ~/esp/esp-idf/export.sh && idf.py build` — build
- `../jettyd-firmware/tools/hil-smoke.sh $(pwd)` — build + flash + boot-check
  on the connected test device (the mandatory PR gate)

## Hardware-in-the-loop gate

Same rules as `jettyd-firmware/CLAUDE.md`: every PR needs an `HIL_SMOKE: PASS`
verdict in the PR body (`## HIL Evidence`), the device lock is exclusive, exit
code 2 means escalate not retry, and feature changes need feature-specific
serial/MQTT evidence beyond the boot smoke. Agents are authorized to flash the
connected device autonomously (Tom, 2026-07-07).

## Do-Not-Touch (escalate instead of editing)

- `partitions.csv` — flash layout changes can strand devices; hold for Tom.
- `sdkconfig.defaults` secrets section (WiFi credentials, fleet token Kconfig
  values) — never commit real credentials.

## Merge policy

- HIL PASS + no partition/secret changes → auto-merge per standard pipeline policy.
- Partition table, OTA, or credential-handling changes → hold for Tom.

## Quality Gates

- test: `bash -c '. ~/esp/esp-idf/export.sh >/dev/null 2>&1 && idf.py build'`
- lint: `python3 build.py device.yaml /tmp/driver_registry_check.c`   # device.yaml → codegen sanity
- typecheck: `true`   # C: covered by the IDF build in `test`

The HIL gate (`../jettyd-firmware/tools/hil-smoke.sh $(pwd)`) is additionally
mandatory before opening a PR — see above.

## Notes

- `sdkconfig` is generated; commit `sdkconfig.defaults` changes only.
- Web-flasher work (ESP Web Tools): manifest + release binaries come from this
  repo's CI; validate a built binary by flashing it with the HIL script before
  publishing a release manifest.
