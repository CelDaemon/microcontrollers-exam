#include <pico/stdlib.h>
#include <stdio.h>
//#include <hardware/gpio.h>
//#include <hardware/adc.h>


int main() {
    stdio_init_all();

    while(true) {
        printf("Hello, World!\n");
        sleep_ms(100);
    }

}
