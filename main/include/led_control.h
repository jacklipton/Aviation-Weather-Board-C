#pragma once

void led_control_init(void);
void led_set_category(const char* category, int index);
void led_turn_off(void);
void led_turn_on_color(const char* color);
