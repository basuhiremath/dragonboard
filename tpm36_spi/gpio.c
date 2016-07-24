/*
 *
 * gpio.c
 * 
 *  Created on: July 10, 2016
 *    Author: basu
 * 
 *    This is based on libsoc https://github.com/jackmitch/libsoc
 *    A stripped down version that does minimum setup.
 *    For a better and robust implemetation, refer to the above github
*/

#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "gpio.h"
#include "my_utils.h"

static const char gpio_direction_strings[GPIO_DIR_MAX][STR_BUF_LEN] = { "in", "out" };
static const char gpio_level_strings[GPIO_LEVEL_MAX][STR_BUF_LEN] = { "0", "1" };

/*request GPIO. Create one if not present*/
int 
gpio_request(unsigned int gpio_num)
{

  int gpio_fd;
  char gpio_file_str[STR_BUF_LEN];
  char temp_str[STR_BUF_LEN];

  /*create a filename using the gpio id*/

  sprintf(temp_str, "/sys/class/gpio/gpio%d/value", gpio_num);

  if ( access(temp_str, F_OK) != -1 )
  {
    printf("GPIO %d already exported\n", gpio_num);
  }
  else
  {

    /*file does not exist, export*/
    printf("GPIO %d does not exist. Will be exported\n", gpio_num);

    int fd = open("/sys/class/gpio/export", O_SYNC | O_WRONLY);

    if (fd < 0)
    {
      printf("cannot open file to export\n");
      return -1;
    }

    sprintf(temp_str, "%d", gpio_num);

    if ( write(fd, temp_str, STR_BUF_LEN) < 0 ) 
    {
      printf("cannot write to file %d for gpio %d\n", fd, gpio_num);

      close(fd);
      return -1;
    } 

    close(fd);

    sprintf(temp_str, "/sys/class/gpio/gpio%d", gpio_num);

    if ( access(temp_str, F_OK) == -1 )
    {
      printf("cannot open file %s\n", temp_str);
      return -1;
    }

  }
  
  /*return the file descriptor*/
  sprintf(gpio_file_str, "/sys/class/gpio/gpio%d/value", gpio_num);

  gpio_fd = open(gpio_file_str, O_SYNC | O_RDWR);
    
  return (gpio_fd);

}


/*release gpio*/
int 
gpio_release(int gpio_value_fd, unsigned int gpio_num)
{

  int fd;
  char temp_str[STR_BUF_LEN];

  if (gpio_value_fd < 0)
  {
    printf("Invalid gpio fd %d\n", gpio_value_fd);
    return -1;
  }

  fd = open("/sys/class/gpio/unexport", O_SYNC | O_WRONLY);

  if (fd < 0)
  {
    printf("Failed to open unexport file\n");
    return -1;
  }

  sprintf(temp_str, "%d", gpio_num);

  if (write( fd, temp_str, STR_BUF_LEN) < 0)
  {
    printf("cannot write to file %d for gpio %d\n", fd, gpio_num);
    close(fd);
    return -1;
  }

  close(fd);

  sprintf(temp_str, "/sys/class/gpio/gpio%d", gpio_num);

  if ( access(temp_str, F_OK) != -1 )
  {
    printf("cannot release file %s\n", temp_str);
    return -1;
  }

  return 0;

}


/*set GPIO direction*/
int 
gpio_set_direction(unsigned int gpio_num, gpio_direction_t gpio_direction)
{


  char temp_str[STR_BUF_LEN];
  int fd;

  if (gpio_direction >= GPIO_DIR_MAX) 
  {
    printf("Invalid GPIO direction type %d\n", gpio_direction);
    return -1;
  }

  sprintf(temp_str, "/sys/class/gpio/gpio%d/direction", gpio_num);

  fd = open(temp_str, O_SYNC | O_WRONLY);

  if (fd < 0)
  {
    printf("Cannot open file %s for gpio %d\n", temp_str, gpio_num);
    return -1;
  }

  if ( write(fd, gpio_direction_strings[gpio_direction], STR_BUF_LEN) < 0 )
  {
    printf("failed to write gpio direction for GPIO %d\n", gpio_num);
    return -1;
  }

  close(fd);

  return 0;

}


/*set GPIO level*/
int 
gpio_set_level(int gpio_value_fd, gpio_level_t gpio_level)
{


  if (gpio_level >= GPIO_LEVEL_MAX) 
  {
    printf("Invalid GPIO level type %d\n", gpio_level);
    return -1;
  }

  if (gpio_value_fd < 0)
  {
    printf("Invalid gpio fd %d\n", gpio_value_fd);
    return -1;
  }


  if ( write(gpio_value_fd, gpio_level_strings[gpio_level], STR_BUF_LEN) < 0 )
  {
    printf("failed to write gpio level for GPIO\n");
    return -1;
  }

  return 0;


}


