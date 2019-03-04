COM0?=/dev/ttyUSB1
COM1?=/dev/ttyUSB3
FBAUD?=460800
SBAUD?=460800
OPTS?=-e -w -v
BOARD?=cc2538dk
APP?=serial-basic
CFLAGS+=

all:
	echo "nothing."

clean:
	make clean -C $(APP)

build:
	make BOARD=$(BOARD) -C $(APP)

flash0:
	./cc2538-bsl.py -p $(COM0) -b $(FBAUD) $(OPTS)  $(APP)/bin/$(BOARD)/$(APP).bin

flash1:
	./cc2538-bsl.py -p $(COM1) -b $(FBAUD) $(OPTS)  $(APP)/bin/$(BOARD)/$(APP).bin

serial0:
	stty -F $(COM0) $(SBAUD) -icrnl min 1 && cat $(COM0)

serial1:
	stty -F $(COM1) $(SBAUD) -icrnl min 1 && cat $(COM1)

