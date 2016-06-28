/*
 * led_hw.h
 *
 *  Created on: Jun 25, 2016
 *      Author: basu
 */

#ifndef LED_HW_H_
#define LED_HW_H_

typedef enum
{
	LED_OFF = 0,
	LED_ON
}led_control_t;


int init_led(void);

int write_led(int data);

int close_led(void);

#endif /* LED_HW_H_ */
