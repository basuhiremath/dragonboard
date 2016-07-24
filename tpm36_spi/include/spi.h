/*
 * spi.h
 *
 *  Created on: July 10, 2016
 *      Author: basu
 */


#ifndef SPI_H_
#define SPI_H_

#define SPI_BITS_PER_WORD_8 8
#define SPI_MAX_SPEED_HZ_10K 10000

typedef enum {

  MY_SPI_MODE_0 = 0,
  MY_SPI_MODE_1,
  MY_SPI_MODE_2,
  MY_SPI_MODE_3,
  MY_SPI_MODE_INVALID
} spi_mode_t;


int spi_init(unsigned int spidev_id, unsigned int chip_select);

int spi_set_mode(int spidev_fd, spi_mode_t my_spi_mode);

int spi_set_bits_per_word(int spidev_0_fd, unsigned int num_bits);

int spi_set_max_speed(int spidev_fd, unsigned int max_speed);

int spi_read_data(int spidev_fd);




#endif /* SPI_H_ */
