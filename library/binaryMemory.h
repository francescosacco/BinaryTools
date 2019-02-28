/**
 * @file binaryMemory.h
 * @author Francesco Sacco
 * @date 28 Feb 2019
 * @brief This library manage binary data at memory dynamically.
 *
 * This library manage binary data at memory dynamically memory reallocation.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-28 - 0.0.0 - Initial version.
  *
  **********/
#ifndef _BINARYMEMORY_H_
#define _BINARYMEMORY_H_

#include <stdint.h>

typedef enum BMRET_T
{
    bmRet_ok       ,
    bmRet_outOfMem ,
    bmRet_memLimit ,
    bmRet_error
} bmRet_t ;

bmRet_t bmInit( void ) ;
bmRet_t bmWrite( uint32_t addr , uint8_t * data , uint32_t size ) ;
bmRet_t bmRead( uint32_t addr , uint8_t * data , uint32_t size ) ;
bmRet_t bmDeInit( void ) ;

uint32_t bmGetSize( void ) ;

#endif // _BINARYMEMORY_H_
