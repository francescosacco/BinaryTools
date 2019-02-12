/**
 * @file base64.c
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
  * 2019-02-12 - 0.1.0 - Created library with Base-64 code.
  * 2018-12-12 - 0.0.0 - Initial version.
  *
  **********/
#include "base32.h"

#include <stdint.h>
#include <string.h>

void bin2base64( uint8_t * binData , uint8_t binDataSize , uint8_t * base64Data )
{
    static const uint8_t base64_table[] = { 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' ,
                                            'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' ,
                                            'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' ,
                                            'Y' , 'Z' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' ,
                                            'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' ,
                                            'o' , 'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' ,
                                            'w' , 'x' , 'y' , 'z' , '0' , '1' , '2' , '3' ,
                                            '4' , '5' , '6' , '7' , '8' , '9' , '+' , '/' } ;
    
    uint8_t data ;
    
    if( base64Data == NULL )
    {
        return ;
    }
    
    memset( base64Data , '\0' , 5 ) ;
    
    switch( binDataSize )
    {
    // 0 Bytes to convert.
    case 0 :
        // There's no data.
        base64Data[ 0 ] = '\0' ;
        break ;
    case 1 :
        // xxxx.xxxx 0000
        // \_____/\_____/
        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data &= 0x30 ;
        base64Data[ 1 ] = base64_table[ data ] ;

        base64Data[ 2 ] = '=' ;
        base64Data[ 3 ] = '=' ;
        break ;
    case 2 :
        // xxxx.xxxx xxxx.xxxx 00
        // \_____/\_____/ \_____/

        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data |= binData[ 1 ] >> 4 ;
        data &= 0x3F ;
        base64Data[ 1 ] = base64_table[ data ] ;

        data  = binData[ 1 ] << 2 ;
        data &= 0x3C ;
        base64Data[ 2 ] = base64_table[ data ] ;

        base64Data[ 3 ] = '=' ;
        break ;
    // 3 bytes to convert.
    default :
        // xxxx.xxxx xxxx.xxxx xxxx.xxxx
        // \_____/\_____/ \_____/\_____/

        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data |= binData[ 1 ] >> 4 ;
        data &= 0x3F ;
        base64Data[ 1 ] = base64_table[ data ] ;

        data  = binData[ 1 ] << 2 ;
        data |= binData[ 2 ] >> 6 ;
        data &= 0x3F ;
        base64Data[ 2 ] = base64_table[ data ] ;

        data  = binData[ 2 ] ;
        data &= 0x3F ;
        base64Data[ 3 ] = base64_table[ data ] ;
        break ;
    }
}
