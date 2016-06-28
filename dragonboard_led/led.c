/*
 * led.c
 *
 *  Created on: Jun 10, 2016
 *      Author: basu
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

#define ERR_FATAL do { fprintf(stderr, "Error: file %s, line %d, error (%d) [%s]\n", \
		__FILE__, __LINE__, errno, strerror(errno)); exit(1); } while(0)

typedef struct
{
	unsigned int cfg;            /*place holder for configuration register*/
	unsigned int in_out;				 /*place holder for IO register*/
}db_gpio_type_s;

int abort_program = 0;

/*GPIOn physical address = 0x01000004 + (0x1000 * n); ref: e peripherals programming guide*/
unsigned int GPIO_120_CFG_ADDR = 0x01000000 + (0x1000 * 120);
unsigned int GPIO_120_IO_ADDR = 0x01000004 + (0x1000 * 120);

void stop_program(int sig)
{
	abort_program = 1;
}

int main(int argc, char *argv[])
{

	db_gpio_type_s *gpio_120;
	int fd_mem;

	/*initialize signal to abort the program*/
	signal(SIGINT, stop_program);

	fd_mem = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd_mem < 0)
	{
		printf("Failed to open /dev/mem. Aborting\n");
		ERR_FATAL;
	}

	gpio_120 = (db_gpio_type_s *)(mmap(0, sizeof(db_gpio_type_s), PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, GPIO_120_CFG_ADDR));

	if (gpio_120 == (void *)-1)
	{
		printf("Failed to map GPIO address. Aborting\n");
		close(fd_mem);
		ERR_FATAL;
	}

	while (!abort_program)
	{
		gpio_120->in_out = gpio_120->in_out | 0x02;
		printf("Contents of physical address 0x%X is 0x%X\n", GPIO_120_IO_ADDR, gpio_120->in_out);
		sleep(1);
		gpio_120->in_out = gpio_120->in_out & ~0x02;
		printf("Contents of physical address 0x%X is 0x%X\n", GPIO_120_IO_ADDR, gpio_120->in_out);
		sleep(1);
	}
	/*clean up and exit*/
	gpio_120->in_out = gpio_120->in_out & ~0x02;

	if (munmap(gpio_120, sizeof(int)) == -1)
	{
		printf("Failed to unmap GPIO address. Aborting\n");
		close(fd_mem);
		ERR_FATAL;
	}

	printf("Contents of physical address 0x%X is 0x%X\n", GPIO_120_IO_ADDR, gpio_120->in_out);

	close (fd_mem);
	printf("\nProgram exiting\n");
	return 0;
}
