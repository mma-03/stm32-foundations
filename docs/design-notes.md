# Design Notes
1. **Behaviour**
    - Holding the button: LED stays on/off
    - On press: toggle LED (turn it on if off, turn it off if on) 
    - On release: nothing happens
    - If the main loop is busy for a second, and the button is pressed,
      it should interrupt and set the flag
    - If the loop is busy and the button is pressed twice in that second, the callback
      sets a bool flag to true, then it sets to true again
    - Two presses during a busy period merge into one toggle
    - Acceptable for a light switch.
2. **Modules + functions**
    - When the button goes from high to low (pressed), it should toggle the LED
    - void led_toggle(void)
    - the flag lives in button.c it is static, and the function returns true or false
    - bool button_was_pressed(void) { returns whether a button press has happened since,
      side effect: answer changes back to false once answer is consumed}
3. **Callback**
    - HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    - HAL passes in which pin triggered it
    - Lives in button.c
    - Encapsulation: button interrupt is a button implementation detail, main should't care
    - only set flag if GPIO_Pin == B1_Pin
4. **The flag**
    - press_pending (flag) lives in button.c
    - It is volatile, since main never touches flag
    - The flag is changed by an interrupt, outside normal program flow, so button_was_pressed() must read it fresh from memory everytime instead of using a stale register copy
    - The callback function sets it and the event function clears it when main asks
5. **main.c**

   Startup:
    - LED is off on startup (led_init())
    - Flags are off at startup (button_init())

   Repeats:
    - Ask the button whether a press has happened. If yes, toggle the LED.


## Consolidated design:

**button.c:**
- static volatile bool flag, starting false
- callback: if the pin is B1, set the flag
- `bool button_was_pressed(void)`: returns the flag and clears it

**led.c**
- add `void led_toggle(void)`

**main.c**
- start up as before: loop: if `button_was_pressed()`, then `led_toggle()`


**button_was_pressed() debugging**

  call stack showed button_was_pressed paused at press_pending = false;
  this means that reading the flag into press_status had already finished when the press arrived.

  1. press_status = press_pending reads false (no press yet)
  2. The press arrives, callback sets press_pending = true
  3. the interrupt returns and main resumes at press_pending = false
  4. the function returns the stale false (the press is lost)

    fix: briefly block interrupts while the flag is being read/cleared
    - __disable_irq() : pauses all interrupts
    - read/clear
    - __enable_irq() : turns interrupts back on