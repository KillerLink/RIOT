DEFAULT_CHANNEL ?= 26
DEFAULT_PANID ?= 0x1337
DEFAULT_TXPOWER ?= 0
IPV6_SERVER_ADDR = '"fd0b:ad46:74e5:de4c::1"'
IPV6_CLIENT_ADDR = '"fd0b:ad46:74e5:de4c::2"'

CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
CFLAGS += -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)
CFLAGS += -DIEEE802154_DEFAULT_TXPOWER=$(DEFAULT_TXPOWER)
CFLAGS += -DIPV6_SERVER_ADDR=$(IPV6_SERVER_ADDR)
CFLAGS += -DIPV6_CLIENT_ADDR=$(IPV6_CLIENT_ADDR)

#CFLAGS += -DGNRC_PKTBUF_SIZE=2048
CFLAGS += -DGNRC_NETIF_IPV6_ADDRS_NUMOF=8
CFLAGS += -DGNRC_NETIF_IPV6_GROUPS_NUMOF=8
CFLAGS += -DGNRC_IPV6_NIB_NUMOF=8
CFLAGS += -DGNRC_IPV6_NIB_OFFL_NUMOF=8

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6
USEMODULE += prng_minstd
USEMODULE += gnrc_udp
USEMODULE += ieee802154
USEMODULE += gnrc_ndp

USEMODULE += gnrc_ipv6_nib
USEMODULE += gnrc_ipv6_nib_6ln
USEMODULE += gnrc_ipv6_nib_6lr

USEMODULE += gnrc_icmpv6
#USEMODULE += gnrc_icmpv6_echo
#USEMODULE += gnrc_icmpv6_error

##USEMODULE += gnrc_pktdump

#USEMODULE += gnrc_sixlowpan
USEMODULE += gnrc_sixlowpan_nd
#USEMODULE += gnrc_sixlowpan_ctx
#USEMODULE += gnrc_sixlowpan_frag
USEMODULE += gnrc_sixlowpan_default
#USEMODULE += gnrc_sixlowpan_router_default

#USEMODULE += gnrc_ipv6_default
#USEMODULE += gnrc_ipv6_router_default
#USEMODULE += gnrc_neterr

#USEMODULE += shell
#USEMODULE += shell_commands
#USEMODULE += ps

USEMODULE += gnrc_sock_udp
