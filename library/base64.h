/**
 * @file base64.h
 * @author Francesco Sacco
 * @date 12 Dec 2018
 * @brief This project converts binary data to Base-64.
 *
 * This library converts binary data to Base-64.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-25 - 0.1.1 - Add more documentation.
  * 2019-02-12 - 0.1.0 - Created library with Base-64 code.
  * 2018-12-10 - 0.0.0 - Initial version.
  *
  **********/
#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdint.h>

void bin2base64( uint8_t * binData , uint8_t binDataSize , uint8_t * base64Data ) ;

#endif // _BASE64_H_
