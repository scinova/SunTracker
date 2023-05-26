NAME = SunTracker
CHIP = esp32
BOARD = nodemcu-32s
BUILD_DIR = build
SKETCH = $(NAME).ino
USE_CCACHE = 1
LIBS = /root/work/SpaceShip/lib
#VERBOSE = 1
include ~/makeEspArduino/makeEspArduino.mk

all:
	cp ${BUILD_DIR}/${NAME}.bin /sdcard/ArduinoDroid/precompiled
