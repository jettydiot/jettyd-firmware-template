.PHONY: setup build flash monitor clean check check-format check-codegen test

SDK_DIR := jettyd-sdk

setup:
	@echo "Cloning jettyd firmware SDK..."
	git clone https://github.com/jettydiot/jettyd-firmware.git $(SDK_DIR)
	@echo "✅ SDK cloned. Now run: make build"

# ── Check — run before every commit ─────────────────────────────────────────
# Validates codegen and catches common issues without a full IDF compile.
# CI runs the full build on every push — this is the fast local gate.

check: check-codegen check-format test
	@echo ""
	@echo "✅ All checks passed — safe to commit"

# ── Unit tests ────────────────────────────────────────────────────────────────
# Runs the SDK host test suite (no ESP-IDF or hardware needed).

test:
	@echo "→ Running SDK unit tests..."
	@$(MAKE) -C $(SDK_DIR)/test --no-print-directory
	@echo "✅ Unit tests OK"

check-codegen:
	@echo "→ Running build.py codegen..."
	@python3 build.py device.yaml main/driver_registry.c
	@test -f main/device_config.h      || (echo "❌ device_config.h not generated" && exit 1)
	@test -f main/driver_registry.c    || (echo "❌ driver_registry.c not generated" && exit 1)
	@test -f main/driver_requires.cmake || (echo "❌ driver_requires.cmake not generated" && exit 1)
	@echo "✅ Codegen OK"

check-format:
	@echo "→ Scanning for uint32_t passed to %%d (Werror=format risk)..."
	@# Strategy: find lines with %d that also reference known uint32_t field names.
	@# uint8_t/uint16_t with %d is fine on xtensa/riscv (promoted to int).
	@# uint32_t is 'long unsigned int' on these targets and must use PRIu32 or %lu.
	@HITS=$$(grep -rn '%[di]' \
		$(SDK_DIR)/drivers/ $(SDK_DIR)/jettyd/src/ 2>/dev/null | \
		grep -v 'PRIu32\|PRId32\|%lu\|%u\|//' | \
		grep -E 'interval_sec|debounce_ms|freq_hz|max_on_duration|press_count|heap_free|uptime') ; \
	if [ -n "$$HITS" ]; then \
		echo "❌ uint32_t field with %%d format — use PRIu32 or (int) cast:"; \
		echo "$$HITS"; \
		exit 1; \
	fi
	@echo "✅ Format specifiers OK"

# ── Build / Flash / Monitor ──────────────────────────────────────────────────

build:
	@rm -f sdkconfig
	idf.py build

flash:
	@rm -f sdkconfig
	idf.py flash

monitor:
	idf.py monitor

flash-monitor:
	@rm -f sdkconfig
	idf.py flash monitor

clean:
	idf.py fullclean
