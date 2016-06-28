/*
 * led_hw.c
 *
 *  Created on: Jun 25, 2016
 *      Author: basu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "led_hw.h"
#include "error_util.h"

/*file system for LED 2*/
#define LED2 "/sys/class/leds/apq8016-sbc\:green\:user2/brightness"

static int fd_led2 = -1;

int init_led(void)
{

	fd_led2 = open(LED2, O_WRONLY);

	if (fd_led2 < 0)
	{
		ERR_FATAL;
		return -1;
	}

	return 0;
}


int write_led(int data)
{

	char buf[1];

	if (fd_led2 < 0)
	{
		printf("file not open to write\n");
		return -1;
	}

	if (data < LED_OFF || data > LED_ON)
	{
		printf("Invalid data %d for led\n", data);
		return -1;

	}

	sprintf(buf, "%d", data);

	if ( write(fd_led2, buf , 1) < 0)
	{
		ERR_FATAL;
		return -1;
	}

	return 0;

}

int close_led(void)
{

	if (fd_led2 < 0)
	{
		printf("file not open to write\n");
		return -1;
	}

	/*clean up and exit*/
	write(fd_led2, "0", 1);
	close (fd_led2);

	return 0;
}
