/*
 * led_sys.c
 *
 *  Created on: Jun 13, 2016
 *      Author: basu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>


#define ERR_FATAL do { fprintf(stderr, "Error: file %s, line %d, error (%d) [%s]\n", \
		__FILE__, __LINE__, errno, strerror(errno)); exit(1); } while(0)

int abort_program = 0;

/*file system for LED 2*/
#define LED2 "/sys/class/leds/apq8016-sbc\:green\:user2/brightness"

void stop_program(int sig)
{
	abort_program = 1;
}


int main(int argc, char *argv[])
{
	int fd_led2;
	/*open the file*/

	/*initialize signal to abort the program*/
	signal(SIGINT, stop_program);

	printf("%s\n", LED2);

	fd_led2 = open(LED2, O_WRONLY);

	if (fd_led2 < 0)
	{
		ERR_FATAL;
		return 1;
	}

	while (!abort_program)
	{
		write(fd_led2, "1", 2);
		sleep(1);
		write(fd_led2, "0", 2);
		sleep(1);
	}

	/*clean up and exit*/
	write(fd_led2, "0", 2);

	close (fd_led2);
	printf("\nProgram exiting\n");
	return 0;



}
