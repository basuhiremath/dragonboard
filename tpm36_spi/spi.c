/*
 *
 * spi.c
 * 
 *  Created on: July 10, 2016
 *    Author: basu
 * 
 *    This is based on following
 *    1. libsoc https://github.com/jackmitch/libsoc
 *    A stripped down version that does minimum setup.
 *    For a better and robust implementation, refer to the above github
 * 
 *    2. spitest.c from MontaVista Software, Inc
 *       Anton Vorontsov <avorontsov@ru.mvista.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/spi/spidev.h>

#include "spi.h"
#include "my_utils.h"

typedef unsigned char uint8_t;


/*initialize spi*/
int 
spi_init(unsigned int spidev_id, unsigned int chip_select)
{
  char spi_dev_str[STR_BUF_LEN];
  int spidev_fd;

  sprintf(spi_dev_str, "/dev/spidev%d.%d", spidev_id, chip_select);

  if ( access(spi_dev_str, F_OK) == -1 ) 
  {
    printf("spidev %s not found\n", spi_dev_str );
    return -1;
  }

  spidev_fd = open(spi_dev_str, O_SYNC | O_RDWR);

  return (spidev_fd);

}


/*set mode for spi*/
int 
spi_set_mode(int spidev_fd, spi_mode_t my_spi_mode)
{

  unsigned int mode;
  int return_value;

  if (spidev_fd < 0)
  {
    printf("Invalid file descriptor %d\n", spidev_fd);
    return -1;
  }

  switch (my_spi_mode)
  {
  case MY_SPI_MODE_0:
    mode = SPI_MODE_0;
    break;
  case MY_SPI_MODE_1:
    mode = SPI_MODE_1;
    break;
  case MY_SPI_MODE_2:
    mode = SPI_MODE_2;
    break;
  case MY_SPI_MODE_3:
    mode = SPI_MODE_3;
    break;
  default:
    printf("Invalid spi mode %d\n", my_spi_mode);
    return -1;
    break;
  }

  return_value = ioctl(spidev_fd, SPI_IOC_WR_MODE, &mode);

  return(return_value);

}


/*set bits per word*/
int 
spi_set_bits_per_word(int spidev_fd, unsigned int num_bits)
{

  int return_value;

  if (spidev_fd < 0)
  {
    printf("Invalid file descriptor %d\n", spidev_fd);
    return -1;
  }

  return_value = ioctl(spidev_fd, SPI_IOC_WR_BITS_PER_WORD, &num_bits);

  if (return_value == -1)
  {
    printf("failed to set write bits for spi at fd %d\n", spidev_fd);
    return (return_value);
  }

  return_value = ioctl(spidev_fd, SPI_IOC_RD_BITS_PER_WORD, &num_bits);

  return (return_value);

}


/*set wr/rd max speed*/
int 
spi_set_max_speed(int spidev_fd, unsigned int max_speed)
{

  int return_value;

  if (spidev_fd < 0)
  {
    printf("Invalid file descriptor %d\n", spidev_fd);
    return -1;
  }

  return_value = ioctl(spidev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed);

  if (return_value == -1)
  {
    printf("failed to set write max speed for spi at fd %d\n", spidev_fd);
    return (return_value);
  }

  return_value = ioctl(spidev_fd, SPI_IOC_RD_MAX_SPEED_HZ, &max_speed);

  return (return_value);

}


/*read data
  adc conversion should not be here. It should have separate file
  Mezzanine card has 10 but adc*/
int spi_read_data(int spidev_fd)
{
	int adc_value;
	int data_length = 3;
	//int i;

	uint8_t tx[3] = {0x01, 0x80, 0x00};
	uint8_t rx[3] = {0x00, 0x00, 0x00};

	struct spi_ioc_transfer tr = {
			.rx_buf = (unsigned long) rx,
			.tx_buf = (unsigned long) tx,
			.len = data_length,
	};

	if ( ioctl(spidev_fd, SPI_IOC_MESSAGE(1), &tr) < 0)
	{
		printf("Communication with SPI %d failed\n", spidev_fd);
		return 0;
	}

	adc_value = (rx[1] << 8) & 0b1100000000;
	adc_value |= rx[2] & 0xff;

	return (adc_value);

}

