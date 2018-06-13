#ifndef __NOTIFY_H_
#define __NOTIFY_H_
#include "stdbool.h"
#include "board_hw_defs.h"

 
extern uint8_t LED_blink_tpye;
 
 
void Init_led_buz_state(void);
void LED_control(void) ;
void Set_led_blink_type(uint8_t tp);
void SetLedColor(uint8_t color);

 
#endif

