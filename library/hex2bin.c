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
#include "hex2bin.h"

#include <stdint.h>
#include <string.h>
  
uint8_t isValidHex( uint8_t * hexIn )
{
    char * validHex = "0123456789abcdefABCDEF" ;
    char * chrRet ;
    
    chrRet = strchr( validHex , ( int ) hexIn[ 0 ] ) ;
    if( chrRet == NULL )
    {
        return( 0 ) ;
    }

    chrRet = strchr( validHex , ( int ) hexIn[ 1 ] ) ;
    if( chrRet == NULL )
    {
        return( 0 ) ;
    }

    return( 1 ) ;
}

uint8_t hex2bin( uint8_t * hexIn )
{
    uint8_t ret = 0x00 ;
    uint8_t chr ;
    
    chr = hexIn[ 0 ] ;

    if( ( chr >= 0x30 ) && ( chr <= 0x39 ) ) // From 0 ... 9
    {
        ret = chr - '0' ;
    }
    else if( ( chr >= 0x41 ) && ( chr <= 0x46 ) ) // From A ... F
    {
        ret = chr - 'A' + 0x0A ;
    }
    if( ( chr >= 0x61 ) && ( chr <= 0x66 ) ) // From a ... f
    {
        ret = chr - 'a' + 0x0A ;
    }
    
    ret <<= 4 ;
    chr = hexIn[ 1 ] ;

    if( ( chr >= 0x30 ) && ( chr <= 0x39 ) ) // From 0 ... 9
    {
        ret |= chr - '0' ;
    }
    else if( ( chr >= 0x41 ) && ( chr <= 0x46 ) ) // From A ... F
    {
        ret |= chr - 'A' + 0x0A ;
    }
    if( ( chr >= 0x61 ) && ( chr <= 0x66 ) ) // From a ... f
    {
        ret |= chr - 'a' + 0x0A ;
    }
    
    return( ret ) ;
}

uint8_t isHexString( uint8_t * str )
{
    uint32_t i ;
    uint8_t chr ;

    /**********
     *
     * |         |  VALID  |         |  VALID  |         |  VALID  |         |
     * |<------->|<------->|<------->|<------->|<------->|<------->|<------->|
     * | 00h  2Fh|30h   39h|3Ah   40h|41h   46h|47h   60h|61h   66h|67h   FFh|
     *
     **********/

    for( i = 0 ; str[ i ] != '\0' ; i++ )
    {
        chr = str[ i ] ;

        // Is it valid?
        if( ( chr >= 0x30 ) && ( chr <= 0x39 ) ) // From 0 ... 9
        {
            // Yes.
            continue ;
        }

        // Is it valid?
        if( ( chr >= 0x41 ) && ( chr <= 0x46 ) ) // From A ... F
        {
            // Yes.
            continue ;
        }

        // Is it valid?
        if( ( chr >= 0x61 ) && ( chr <= 0x66 ) ) // From a ... f
        {
            // Yes.
            continue ;
        }

        // If so, it's not valid.
        return( 0 ) ;
    }

    return( 1 ) ;
}

