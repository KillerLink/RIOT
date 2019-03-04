#include <stdio.h>
#include <inttypes.h>
#include "net/sock/udp.h"
#include "../common/common.h"


#define MSG_QUEUE_SIZE (1<<6)


static msg_t msg_queue[MSG_QUEUE_SIZE];
static char receive_buffer[DATA_LENGTH];
static sock_udp_ep_t server = SOCK_IPV6_EP_ANY;
static sock_udp_t sock;
static sock_udp_ep_t client;

void panic (void) {
	printf("panic spin\n");
	EVAL_LED4_ON;
	while(1);
}

void init (void) {
	msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
	EVAL_LED1_OUTPUT;
	EVAL_LED2_OUTPUT;
	EVAL_LED4_OUTPUT;
	EVAL_LED1_OFF;
	EVAL_LED2_OFF;
	EVAL_LED4_OFF;
}

void recv_data(void) {
	int res;
	int bytesReceived = 0;
	int errorCount = 0;
	while (bytesReceived < DATA_TOTAL) {
		res = sock_udp_recv(&sock, receive_buffer, DATA_LENGTH, SOCK_NO_TIMEOUT, &client);
		EVAL_LED2_TOGGLE;
		if (res < 0) {
			errorCount++;
		} else {
			bytesReceived+=res;
		}
//		printf("(progress) bytesReceived = %d, errorCount = %d (last res = %d)\n", bytesReceived, errorCount, res);
	}
	EVAL_LED1_TOGGLE;
	printf("rx: %d, err: %d\n", bytesReceived, errorCount);
}

void start_udp (void) {
	server.port = SERVER_PORT;
	if (sock_udp_create(&sock, &server, NULL, 0) < 0) {
		printf("Error: could not create socket");
		panic();
	}
}

int main(void) {
	init();
	// get interfaces, print their addresses, add custom address
	gnrc_netif_t *netif = NULL;
	while ((netif = gnrc_netif_iter(netif))) {
		{
			ipv6_addr_t myaddr;
			ipv6_addr_from_str(&myaddr, IPV6_SERVER_ADDR);
			gnrc_netif_ipv6_addr_add(netif, &myaddr, 64, 0);
			ipv6_addr_from_str(&myaddr, "ff02::1:ff00:0001");
			gnrc_netif_ipv6_group_join(netif, &myaddr);
			ipv6_addr_from_str(&myaddr, "ff02::1"); //all nodes
			gnrc_netif_ipv6_group_join(netif, &myaddr);
			ipv6_addr_from_str(&myaddr, "ff02::2"); //all routers
			gnrc_netif_ipv6_group_join(netif, &myaddr);
		}
	}
	printf("==== Network Configuration ====\n");
	while ((netif = gnrc_netif_iter(netif))) {
		int res;
		ipv6_addr_t ipv6_addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
		char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];
		//gnrc_ipv6_nib_init_iface (netif);
		printf("\tInterface PID: %d\n", netif->pid);
		res = gnrc_netif_ipv6_addrs_get(netif, ipv6_addrs, sizeof(ipv6_addrs));
		for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
			ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
			printf("\t\tAddress: %s\n", ipv6_addr);
		}
		res = gnrc_netif_ipv6_groups_get(netif, ipv6_addrs, sizeof(ipv6_addrs));
		for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
			char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];
			ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
			printf("\t\tGroup: %s\n", ipv6_addr);
		}
	}
	printf("===============================\n");

	xtimer_usleep(1000*1000);

	start_udp();
	while (true) {
		recv_data();
	}
	return 0;
}
