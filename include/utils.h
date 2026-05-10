/**
 * @file utils.h
 * @author Francesco Sacco
 * @date 10 May 2026
 * @brief This library manage the conversion strings.
 *
 * This library was developed to convert strings from different
 * data types.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-05-10 - 0.0.0 - Initial version.
  *
  **********/

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int parse_number( const char * str , long * value ) ;

#endif // UTILS_H
