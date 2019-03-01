/**
 * @file hexString.h
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
  * 2019-03-01 - 0.2.0 - Organized name and functions.
  * 2019-02-11 - 0.1.0 - Created library with hex2bin code.
  * 2019-02-09 - 0.0.1 - Fix upper case.
  * 2019-02-08 - 0.0.0 - Initial version.
  *
  **********/
#include "hex2bin.h"

#include <stdint.h>
#include <string.h>
  
uint8_t isValidHex( char * hexIn )
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

uint8_t hex2bin( char * hexIn )
{
    uint8_t ret = 0x00 ;
    uint8_t chr ;
    
    chr = ( uint8_t ) hexIn[ 0 ] ;

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
