.PHONY: setup build flash monitor clean

SDK_DIR := jettyd-sdk

setup:
	@echo "Cloning jettyd firmware SDK..."
	git clone https://github.com/jettydiot/jettyd-firmware.git $(SDK_DIR)
	@echo "✅ SDK cloned. Now run: idf.py build"

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
