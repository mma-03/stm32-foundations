#ifndef _BUTTON_H
#define _BUTTON_H

#include <stdbool.h>

void button_init(void);

bool button_is_pressed(void);

bool button_was_pressed(void);

#endif