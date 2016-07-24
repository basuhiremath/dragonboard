/*
 * gpio.h
 *
 *  Created on: July 10, 2016
 *      Author: basu
 */

#ifndef GPIO_H_
#define GPIO_H_


typedef enum {

  GPIO_DIR_IN = 0,
  GPIO_DIR_OUT,
  GPIO_DIR_MAX,
  GPIO_DIR_INVALID = GPIO_DIR_MAX
} gpio_direction_t;


typedef enum {

  GPIO_LEVEL_LOW = 0,
  GPIO_LEVEL_HIGH,
  GPIO_LEVEL_MAX,
  GPIO_LEVEL_INVALID = GPIO_LEVEL_MAX
} gpio_level_t;


int gpio_request(unsigned int gpio_num);

int gpio_release(int gpio_value_fd, unsigned int gpio_num);

int gpio_set_direction(unsigned int gpio_num, gpio_direction_t gpio_direction);

int gpio_set_level(int gpio_value_fd, gpio_level_t gpio_level);


#endif /* GPIO_H_ */
