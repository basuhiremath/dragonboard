/*
 * my_errors.h
 *
 *  Created on: July 10, 2016
 *      Author: basu
 */

#ifndef MY_ERRORS_H_
#define MY_ERRORS_H_


#include <errno.h>

#define ERR_FATAL do { fprintf(stderr, "Error: file %s, line %d, error (%d) [%s]\n", \
		__FILE__, __LINE__, errno, strerror(errno)); exit(1); } while(0)

#define CHECK_ERROR if (error) { \
        printf ("%s\n", strerror (error)); \
        return 1; }

#endif /* MY_ERRORS_H_ */


