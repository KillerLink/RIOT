BOARD ?= b-l072z-lrwan1
include ../Makefile.tests_common

FEATURES_REQUIRED += periph_eeprom

BOARD_INSUFFICIENT_MEMORY := arduino-duemilanove arduino-uno

USEMODULE += shell
USEMODULE += shell_commands  # provides reboot command

include $(RIOTBASE)/Makefile.include
