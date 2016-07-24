

/*
 *
 * tmp36_spi.c
 * 
 *  Created on: July 10, 2016
 *    Author: basu
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>

#include "spi.h"
#include "gpio.h"
#include "my_errors.h"

#define GPIO_18_CS 18
#define SPIDEV_0 0

int abort_program = 0;
pthread_t read_temperature_thread;

int gpio_18_cs_value_fd;
int spidev_0_fd;


/*abort program on ctrl+c*/
void 
stop_program(int sig)
{
	abort_program = 1;
}


/*thread to read the temperature*/
void 
*read_temperature(void *arg)
{

  int adc_value;
  float temp_in_C;
  float	temp_in_F;

  printf("Executing thread\n");

  while(1)
  {
    gpio_set_level(gpio_18_cs_value_fd, GPIO_LEVEL_HIGH);
    usleep(10);
    gpio_set_level(gpio_18_cs_value_fd, GPIO_LEVEL_LOW);

    adc_value = spi_read_data(spidev_0_fd);

    gpio_set_level(gpio_18_cs_value_fd, GPIO_LEVEL_HIGH);

    temp_in_C = (adc_value*5.0/1023-0.5)*100;
    temp_in_F = temp_in_C * 9/5 + 32;

    printf("The current temperature is %2.1f C / %2.1f F \n", temp_in_C, temp_in_F);

    sleep(2);

  }

}


/*temperature read thread*/
int 
create_temperature_thread(void)
{

	int error;
	error = pthread_create(&read_temperature_thread, NULL, read_temperature, NULL);
	CHECK_ERROR;

	printf("thread created\n");

	return 0;
}


/*terminate thread*/
void 
terminate_temperature_thread(void)
{

	void *ret_value;
	pthread_cancel(read_temperature_thread);
	pthread_join(read_temperature_thread, &ret_value);

	printf("thread terminated\n");

}


/*setup peripherals and spin thread to read temperature*/
int 
main(int argc, char *argv[])
{


  /*initialize signal to abort the program*/
  signal(SIGINT, stop_program);

  /* Setup GPIO */
  /*-------------------------------------------------------------------------*/
  /*Request GPIO*/
  gpio_18_cs_value_fd = gpio_request(GPIO_18_CS);
  if ( gpio_18_cs_value_fd < 0)
  {
    printf("Failed to open GPIO %d\n", GPIO_18_CS);
    return -1;
  }

  /*Set the direction to GPIO*/
  if ( gpio_set_direction(GPIO_18_CS, GPIO_DIR_OUT) < 0)
  {
    printf("Failed to set direction for  GPIO %d\n", GPIO_18_CS);
    return -1;
  }

  /*-------------------------------------------------------------------------*/


  /*Setup SPI for adc*/
  /*-------------------------------------------------------------------------*/
  /*Initialize SPI*/
  spidev_0_fd = spi_init(SPIDEV_0, 0);
  if (spidev_0_fd < 0)
  {
    printf("Failed to initialize SPI %d\n", SPIDEV_0);
    return -1;
  }

  if ( spi_set_mode(spidev_0_fd, MY_SPI_MODE_0) < 0) 
  {
    printf("Failed to set SPI %d mode\n", SPIDEV_0);
    return -1;
  }

  if ( spi_set_bits_per_word(spidev_0_fd, SPI_BITS_PER_WORD_8) )
  {
    printf("Failed to set SPI %d bits per word\n", SPIDEV_0);
    return -1;
  }

  if ( spi_set_max_speed(spidev_0_fd, SPI_MAX_SPEED_HZ_10K) )
  {
    printf("Failed to set SPI %d max speed\n", SPIDEV_0);
    return -1;
  }

  /*-------------------------------------------------------------------------*/

  /*Crete thread for the temperature read*/
  if (create_temperature_thread() == 1 )
  {
    printf("Cannot create thread. Aborting\n");
    abort_program = 1;
  }
  /*Read ADC*/
  /*-------------------------------------------------------------------------*/
  while (!abort_program)
  {

  }
  /*-------------------------------------------------------------------------*/


  /*Exit cleanup*/
  /*-------------------------------------------------------------------------*/
  /*reset and close*/
  terminate_temperature_thread();
  
  gpio_set_level(gpio_18_cs_value_fd, GPIO_LEVEL_LOW);
  gpio_release(gpio_18_cs_value_fd, GPIO_18_CS);
  close(gpio_18_cs_value_fd);

  close(spidev_0_fd);
  /*-------------------------------------------------------------------------*/

  return 0;

}
