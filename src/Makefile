# Target specific macros
TARGET = lego_konvoj
TARGET_SOURCES := \
	$(TARGET).c
TOPPERS_OSEK_OIL_SOURCE := ./$(TARGET).oil

O_PATH ?= build

#include all the build logic
include ../../../dev/nxtosek/nxtOSEK/ecrobot/ecrobot.mak

NeXTTool:
	echo "Flyttar NeXTTool så vi kan flasha över filer"
	cp ../../../dev/nxtosek/NeXTTool ./NeXTTool

flash: all NeXTTool
	echo "sudo ./NeXTTool /COM=usb -download=./$(TARGET).rxe"
	sudo ./NeXTTool /COM=usb -download=./$(TARGET).rxe