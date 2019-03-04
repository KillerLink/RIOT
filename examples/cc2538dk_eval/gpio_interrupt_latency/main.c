#include <stdio.h>
#include "periph/gpio.h"

gpio_t led_1 = GPIO_PIN(2,0);
gpio_t led_2 = GPIO_PIN(2,1);
gpio_t led_3 = GPIO_PIN(2,2);
gpio_t led_4 = GPIO_PIN(2,3);
gpio_t key_left = GPIO_PIN(2,4);
gpio_t key_right= GPIO_PIN(2,5);
gpio_t key_up = GPIO_PIN(2,6);
gpio_t key_down = GPIO_PIN(2,7);
gpio_t key_sel = GPIO_PIN(0,3);

int error(void) {
	while (true) {
	    gpio_init(led_4, GPIO_OUT);
		gpio_write(led_4, true);
	}
}

void gpio_cb(void* arg) {
	(void)(arg);
	gpio_toggle(led_1);
}

int main(void)
{

    gpio_init(led_1, GPIO_OUT);
	gpio_init_int(key_sel, GPIO_IN_PU, GPIO_BOTH, (gpio_cb_t)gpio_cb, (void*)0);
	while(true) {
		//nothing
	}
    return 0;
}
