#include <stdio.h>
#include "xtimer.h"

int main(void) {
	while (true) {
		printf("Hello World!\n");
		xtimer_sleep(1);
	}
	return 0;
}
