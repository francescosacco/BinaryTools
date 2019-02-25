/**
 * @file base32.h
 * @author Francesco Sacco
 * @date 12 Dec 2018
 * @brief This project converts binary data to Base-32.
 *
 * This library converts binary data to Base-32.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-25 - 0.1.1 - Add more documentation and fix conversion.
  * 2018-12-30 - 0.1.0 - Created library with Base-32 code.
  * 2018-12-12 - 0.0.0 - Initial version.
  *
  **********/
#ifndef _BASE32_H_
#define _BASE32_H_

#include <stdint.h>

void bin2base32( uint8_t * binData , uint8_t binDataSize , uint8_t * base32Data ) ;

#endif // _BASE32_H_
