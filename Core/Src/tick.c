#include "main.h"
#include "tick.h"
#include <stdbool.h>

extern TIM_HandleTypeDef htim3;

static volatile bool tick_pending = false; // flag

void tick_init(void) {
    // start the timer
    if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3) {
        tick_pending = true;
    }
}

// event: "has a tick happened yet?"
bool tick_elapsed(void) {
    __disable_irq();
    bool tick_status = tick_pending;
    tick_pending = false;
    __enable_irq();
    return tick_status;
}