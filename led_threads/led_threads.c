/*
 * led_threads.c
 *
 *  Created on: Jun 25, 2016
 *      Author: basu
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "led_hw.h"
#include "error_util.h"


int abort_program = 0;
pthread_t blink_led_thread;


void exit_program(int sig)
{
	abort_program = 1;
}


void *blink_led(void *arg)
{

	printf("Executing thread\n");

	while(1)
	{
		if (write_led(LED_ON) < 0)
		{
			printf("Write to LED failed. Aborting\n");
			abort_program = 1;
		}
		sleep(1);

		if (write_led(LED_OFF) < 0)
		{
			printf("Write to LED failed. Aborting\n");
			abort_program = 1;
		}
		sleep(1);
	}

}

int create_led_thread(void)
{

	int error;
	error = pthread_create(&blink_led_thread, NULL, blink_led, NULL);
	CHECK_ERROR;

	printf("thread created\n");

	return 0;
}

void terminate_led_thread(void)
{

	void *ret_value;
	pthread_cancel(blink_led_thread);
	pthread_join(blink_led_thread, &ret_value);

	printf("thread terminated\n");

}


int main(int argc, char *argv[])
{

	/*initialize signal to abort the program*/
	signal(SIGINT, exit_program);

	/*open the file*/
	if ( init_led() < 0 )
	{
		printf("LED init failed\n");
		abort_program = 1;
	}

	/*thread to blnk led*/
	if (create_led_thread() == 1 )
	{
		printf("Cannot create thread. Aborting\n");
		abort_program = 1;
	}

	/*loop until called to abort*/
	while (!abort_program)
	{

	}

	/*Clean up and exit*/
	terminate_led_thread();
	close_led();

	printf("\nProgram exiting\n");
	return 0;

}

