#include <stdio.h>
#include <inttypes.h>
#include "net/sock/udp.h"
#include "../common/common.h"


static const char lorem_ipsum[] =
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr,"
" sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum.\n"
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor"
" invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum.\n"
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor"
" invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
"At vero eos et accusam et justo duo dolores et ea rebum."
"Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
"\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat,"
" vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim"
" qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.\n"
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod"
" tincidunt ut laoreet dolore magna aliquam erat volutpat.\n"
"\n"
"Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis"
" nisl ut aliquip ex ea commodo consequat.\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat,"
" vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio"
" dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.\n"
"\n"
"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming"
" id quod mazim placerat facer possim assum.\n"
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
" sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.\n"
;

#define MSG_QUEUE_SIZE (1<<6)


static msg_t msg_queue[MSG_QUEUE_SIZE];


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

sock_udp_t sock;
sock_udp_ep_t server = {.port = SERVER_PORT, .family = AF_INET6 };
sock_udp_ep_t client = SOCK_IPV6_EP_ANY;


void start_udp (void) {
	client.port = SERVER_PORT;
    if (sock_udp_create(&sock, &client, NULL, 0) < 0) {
        printf("Error: failed to create socket");
		panic();
    }
	ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, IPV6_SERVER_ADDR);
	if ((ipv6_addr_t *)&server.addr.ipv6 == NULL) {
		printf("Error: unable to parse destination address\n");
		panic();
	}
}

void send_data (void) {
	int ret;
	int bytesSent = 0;
	int errorCount = 0;
	while (bytesSent < DATA_TOTAL) {
		ret = sock_udp_send(&sock, lorem_ipsum, DATA_LENGTH, &server);
		if (ret <= 0) {
			errorCount++;
		} else {
			EVAL_LED2_TOGGLE;
			bytesSent+=ret;
		}
	}
	EVAL_LED1_TOGGLE;
	printf("bytesSent = %d, errorCount = %d\n", bytesSent, errorCount);
}

int main(void) {
	init();
	// get interfaces, print their addresses, add custom address
	gnrc_netif_t *netif = NULL;
	while ((netif = gnrc_netif_iter(netif))) {
		{
			ipv6_addr_t myaddr;
			ipv6_addr_from_str(&myaddr, IPV6_CLIENT_ADDR);
			gnrc_netif_ipv6_addr_add(netif, &myaddr, 64, 0);
			ipv6_addr_from_str(&myaddr, "ff02::1:ff00:0002");
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
			ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
			printf("\t\tGroup: %s\n", ipv6_addr);
		}
/*
		res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs, sizeof(ipv6_addrs));
		if (res < 0) {
			continue;
		}
		for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
			char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];
			ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
			printf("\t\tAddress: %s\n", ipv6_addr);
		}
*/
	}
	printf("===============================\n");

	xtimer_usleep(1000*1000);

	start_udp();
	send_data();
	while (true) {
		//nothing, end
	}
    sock_udp_close(&sock);

	return 0;
}
