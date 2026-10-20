#include "main.h"
#include "led.h"

void led_init(void) {
    // initialize with led off
    led_off();
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void led_on(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void led_off(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}
