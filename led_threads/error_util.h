/*
 * error_util.h
 *
 *  Created on: Jun 25, 2016
 *      Author: basu
 */

#ifndef ERROR_UTIL_H_
#define ERROR_UTIL_H_


#define ERR_FATAL do { fprintf(stderr, "Error: file %s, line %d, error (%d) [%s]\n", \
		__FILE__, __LINE__, errno, strerror(errno)); exit(1); } while(0)

#define CHECK_ERROR if (error) { \
        printf ("%s\n", strerror (error)); \
        return 1; }

#endif /* ERROR_UTIL_H_ */
