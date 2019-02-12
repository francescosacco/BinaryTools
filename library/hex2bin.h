/**
 * @file hex2bin.h
 * @author Francesco Sacco
 * @date 08 Feb 2018
 * @brief This library converts HEX data to binary data.
 *
 * This software is a tool to convert HEX data to binary string
 * data.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-11 - 0.1.0 - Created library with hex2bin code.
  * 2019-02-09 - 0.0.1 - Fix upper case.
  * 2019-02-08 - 0.0.0 - Initial version.
  *
  **********/
#ifndef _HEX2BIN_H_
#define _HEX2BIN_H_

#include <stdint.h>

uint8_t isValidHex( uint8_t * hexIn ) ;
uint8_t hex2bin( uint8_t * hexIn ) ;

#endif // _HEX2BIN_H_
