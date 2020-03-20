CFLAGS= -Wall
LDLIBS = -lusb-1.0

#lsusb
#Bus 001 Device 007: ID 04b4:8613 Cypress Semiconductor Corp. CY7C68013 EZ-USB FX2 USB 2.0 Development Kit
#Bus 001 Device 011: ID 0925:3881 Lakeview Research Saleae Logic

VID = 04b4
PID = 8613

#VID = 0925
#PID = 3881

#sigrok-fx2-8ch
#VID = 1d50
#PID = 608c

ID = $(VID):$(PID)

USB_BUS = $(shell lsusb | grep $(ID) | awk '{print $$2}')
USB_DEV = $(shell lsusb | grep $(ID) | awk '{print $$4}' | tr -d ':')

${warning $(USB_BUS) $(USB_DEV)}

DEVICE = /dev/bus/usb/$(USB_BUS)/$(USB_DEV)

all: fx2eeprom

load:
	fxload -I vend_ax.hex -D $(DEVICE) -t fx2lp -v

dump:
	./fx2eeprom r 0x$(VID) 0x$(PID) 16 > dump.bin
	@xxd -g 1 dump.bin

write_saleae:
	cat ../../firmware/saleae.iic | ./fx2eeprom w 0x$(VID) 0x$(PID) 16 > write.bin
	@xxd -g 1 write.bin

write_fx2lafw:
	cat ../../firmware/fx2lafw-sigrok-fx2-8ch-new.iic | ./fx2eeprom w 0x$(VID) 0x$(PID) 16 > write.bin
	@xxd -g 1 write.bin

erase:
	cat ../../firmware/erase.bin | ./fx2eeprom w 0x$(VID) 0x$(PID) 16 > write.bin

clean:
	${RM} fx2eeprom
