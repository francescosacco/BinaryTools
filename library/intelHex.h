/**
 * @file intelHex.h
 * @author Francesco Sacco
 * @date 28 Feb 2019
 * @brief This library convert Intel Hex strings.
 *
 * This library can convert Intel hex strings to binary data.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-28 - 0.0.0 - Initial version.
  *
  **********/
#ifndef _INTELHEX_H_
#define _INTELHEX_H_

#include <stdint.h>

typedef enum IHEXTYPE_T
{
    iHexType_data         = 0x00 ,
    iHexType_endOfFile    = 0x01 ,
    iHexType_extSegAddr   = 0x02 ,
    iHexType_startSegAddr = 0x03 ,
    iHexType_extLinAddr   = 0x04 ,
    iHexType_startLinAddr = 0x05
} iHexType_t ;

typedef struct IHEX_T
{
    uint8_t    startCode   ;
    uint8_t    byteCount   ;
    uint16_t   addr        ;
    iHexType_t recType     ;
    uint8_t    data[ 255 ] ;
    uint8_t    checkSum    ;
} iHex_t ;

typedef enum IHEXRET_T
{
    iHexRet_ok ,
    iHexRet_error
} iHexRet_t ;

iHexRet_t iHexToBin( char * strIn , iHex_t * iHex_stOut ) ;
iHexRet_t binToiHex( iHex_t * iHex_stIn , char * strOut ) ;

#endif // _INTELHEX_H_
