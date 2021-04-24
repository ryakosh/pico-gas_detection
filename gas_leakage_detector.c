#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define gas_detector_pin 0
#define buzzer_pin 1
#define stop_btn 2

int64_t three_second_passed_alarm(alarm_id_t alarm_id, void *user_data);

volatile bool fire = false;
volatile bool did_call_alarm = false;

void initialize_pins() {
    // Gas detector pin initialization
    gpio_init(gas_detector_pin);
    gpio_set_dir(gas_detector_pin, false);
    gpio_pull_down(gas_detector_pin);

    // Buzzer pin initialization
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, true);
    gpio_pull_down(buzzer_pin);

    // Stop button pin initialization
    gpio_init(stop_btn);
    gpio_set_dir(stop_btn, false);
    gpio_pull_down(stop_btn);
}

void irq_callback(uint pin, uint32_t events) {
    if(pin == gas_detector_pin) {
        if(!did_call_alarm) {
            add_alarm_in_ms(3000, &three_second_passed_alarm, NULL, false);
            did_call_alarm = true;
        }
    } else if(pin == stop_btn) {
        did_call_alarm = false;
        fire = false;
    }
}

int64_t three_second_passed_alarm(alarm_id_t alarm_id, void *user_data) {
    if(!gpio_get(gas_detector_pin)) {
        fire = true;
    }
    did_call_alarm = false;

    return 0;
}

int main() {
    stdio_init_all();
    initialize_pins();
    alarm_pool_init_default();

    gpio_set_irq_enabled_with_callback(gas_detector_pin, GPIO_IRQ_EDGE_FALL, true, &irq_callback);
    gpio_set_irq_enabled_with_callback(stop_btn, GPIO_IRQ_EDGE_RISE, true, &irq_callback);
    
    while(true) {
        if(fire) {
            gpio_put(buzzer_pin, 1);
            sleep_ms(250);
            gpio_put(buzzer_pin, 0);
            sleep_ms(250);
        }
    }
}