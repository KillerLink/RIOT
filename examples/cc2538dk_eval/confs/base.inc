# If no BOARD is found in the environment, use this default:
BOARD ?= cc2538dk

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

CFLAGS += -DKEEP_LEDS_FREE
CFLAGS += -DKERNEL_CONF_STACKSIZE_MAIN=2048
CFLAGS += -DKERNEL_CONF_STACKSIZE_DEFAULT=2048
CFLAGS += -DKERNEL_CONF_STACKSIZE_PRINTF=2048
CFLAGS += -DSTDIO_UART_BAUDRATE=460800
CFLAGS += -DHOT_PATCH=0
