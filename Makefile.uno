NAME = SunTracker
BOARD_TAG = uno
ARDUINO_DIR = /usr/share/arduino
ARDMK_DIR = /usr/share/arduino
ARDUINO_LIBS = TinyGPSPlus LiquidCrystal
USER_LIB_PATH = /root/Arduino/libraries
include ${ARDMK_DIR}/Arduino.mk

all:
	cp build-${BOARD_TAG}/${NAME}.hex /sdcard/ArduinoDroid/precompiled
