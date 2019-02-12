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
    
    if( ( hexIn[ 0 ] >= '0' ) && ( hexIn[ 0 ] <= '9' ) )
    {
        ret = hexIn[ 0 ] - '0' ;
    }
    else if( ( hexIn[ 0 ] >= 'A' ) && ( hexIn[ 0 ] <= 'F' ) )
    {
        ret = hexIn[ 0 ] - 'A' + 0x0A ;
    }
    else if( ( hexIn[ 0 ] >= 'a' ) && ( hexIn[ 0 ] <= 'f' ) )
    {
        ret = hexIn[ 0 ] - 'a' + 0x0A ;
    }
    
    ret <<= 4 ;

    if( ( hexIn[ 1 ] >= '0' ) && ( hexIn[ 1 ] <= '9' ) )
    {
        ret |= hexIn[ 1 ] - '0' ;
    }
    else if( ( hexIn[ 1 ] >= 'A' ) && ( hexIn[ 1 ] <= 'F' ) )
    {
        ret |= hexIn[ 1 ] - 'A' + 0x0A ;
    }
    else if( ( hexIn[ 1 ] >= 'a' ) && ( hexIn[ 1 ] <= 'f' ) )
    {
        ret |= hexIn[ 1 ] - 'a' + 0x0A ;
    }

    return( ret ) ;
}
