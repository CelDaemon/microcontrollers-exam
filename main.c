#include <pico/stdlib.h>
#include <stdio.h>
#include <hardware/gpio.h>

#define BIT(x) (1u << (x))

#define ACTIVATE_TIMEOUT (10 * 1000 * 1000)

#define BUTTON_PIN 21

#define STATUS_PIN 20

#define STRENGTH1_PIN 19
#define STRENGTH2_PIN 18
#define STRENGTH3_PIN 17

#define TRIGGER_PIN 2
#define ECHO_PIN 3

absolute_time_t last_activated = 0;
volatile absolute_time_t echo_started = 0;
volatile float distance_mm = -1.f;
volatile bool triggered = false;

bool is_activated() {
    if(is_nil_time(last_activated))
        return false;
    return absolute_time_diff_us(last_activated, get_absolute_time()) < ACTIVATE_TIMEOUT;
}

void trigger() {
    triggered = true;
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);
}

void echo_irq() {
    if(gpio_get(ECHO_PIN)) {
        echo_started = get_absolute_time();
        return;
    }
    distance_mm = absolute_time_diff_us(echo_started, get_absolute_time()) / 5.8773f;
    triggered = false;
}

int get_strength_index(float distance_mm) {
    if(distance_mm < 0)
        return 0;
    if(distance_mm <= 100)
        return 3;
    if(distance_mm <= 200)
        return 2;
    if(distance_mm <= 300)
        return 1;
    return 0;
}

#define INPUT_PINS (BIT(BUTTON_PIN) | BIT(ECHO_PIN))

#define OUTPUT_PINS (BIT(STATUS_PIN) | BIT(STRENGTH1_PIN) | BIT(STRENGTH2_PIN) | BIT(STRENGTH3_PIN) | BIT(TRIGGER_PIN))

#define PINS (INPUT_PINS | OUTPUT_PINS)

int main() {
    stdio_init_all();

    gpio_init_mask(PINS);

    gpio_set_dir_masked(INPUT_PINS, 0);
    gpio_set_dir_masked(OUTPUT_PINS, ~0);

    gpio_pull_up(BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(ECHO_PIN, (GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL), true, echo_irq);

    float min_distance_mm = -1;
    while(true) {
        bool status = false;
        int strength_index = 0;
        if(!gpio_get(BUTTON_PIN))
            last_activated = get_absolute_time();
        if(is_activated()) {
            status = true;
            if(!triggered)
                trigger();
            float cached_distance_mm = distance_mm;
            if(min_distance_mm == -1 || cached_distance_mm < min_distance_mm)
                min_distance_mm = cached_distance_mm;
            strength_index = get_strength_index(cached_distance_mm);
        } else if(!is_nil_time(last_activated)) {
            last_activated = 0;
            printf("De opgeslagen afstand is: %f mm\n", min_distance_mm);
            min_distance_mm = -1;
        }
        gpio_put(STATUS_PIN, status);
        gpio_put(STRENGTH1_PIN, strength_index > 0);
        gpio_put(STRENGTH2_PIN, strength_index > 1);
        gpio_put(STRENGTH3_PIN, strength_index > 2);

        // sleep_ms(1);
    }

}
