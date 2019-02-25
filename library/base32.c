/**
 * @file base32.c
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
#include "base32.h"

#include <stdint.h>
#include <string.h>

void bin2base32( uint8_t * binData , uint8_t binDataSize , uint8_t * base32Data )
{
    // Bitmap                               xx000  xx001  xx010  xx011  xx100  xx101  xx110  xx111
    static const uint8_t base32_table[] = {   'A' ,  'B' ,  'C' ,  'D' ,  'E' ,  'F' ,  'G' ,  'H' ,   // 00xxx
                                              'I' ,  'J' ,  'K' ,  'L' ,  'M' ,  'N' ,  'O' ,  'P' ,   // 01xxx
                                              'Q' ,  'R' ,  'S' ,  'T' ,  'U' ,  'V' ,  'W' ,  'X' ,   // 10xxx
                                              'Y' ,  'Z' ,  '2' ,  '3' ,  '4' ,  '5' ,  '6' ,  '7' } ; // 11xxx

    
    uint8_t binDataCopy[ 5 ] ;
    uint8_t data , binDataCopySize ;
    
    if( ( base32Data == NULL ) || ( binDataSize == 0 ) )
    {
        return ;
    }

    memset( base32Data  , 0x00 ,                     9 ) ;
    memset( binDataCopy , 0x00 , sizeof( binDataCopy ) ) ;
    binDataCopySize = ( binDataSize > 5 ) ? ( 5 ) : ( binDataSize ) ;
    memcpy( binDataCopy , binData , binDataCopySize ) ;

    /**********
     *
     * Byte 0 - bits 7 ~ 3
     *
     * | Byte 0
     * | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+
     *  \________ ________/
     *           |
     *           +-----> Data 0
     *
     **********/
    data  = binDataCopy[ 0 ] >> 3 ; // xxx76543
    data &= 0x1F                  ; // ...76543
    base32Data[ 0 ] = base32_table[ data ] ;
    
    /**********
     *
     * Byte 0 - bits 2 ~ 0
     * Byte 1 - bits 7 ~ 6
     *
     * | Byte 0                        | Byte 1
     * | 7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *                      \________ ________/
     *                               |
     *                               +-----> Data 1
     *
     **********/
    data  = binDataCopy[ 0 ] << 2 ; // 543210xx
    data |= binDataCopy[ 1 ] >> 6 ; // xxxxxx76
    data &= 0x1F                  ; // ...21076
    base32Data[ 1 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 1 - bits 5 ~ 1
     *
     * | Byte 1
     * | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+
     *          \________ ________/
     *                   |
     *                   +-----> Data 2
     *
     **********/
    data  = binDataCopy[ 1 ] >> 1 ; // x7654321
    data &= 0x1F                  ; // ...54321
    base32Data[ 2 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 1 - bit 0
     * Byte 2 - bits 7 ~ 4
     *
     * | Byte 1                        | Byte 2
     * | 7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *                              \________ ________/
     *                                       |
     *                                       +-----> Data 3
     *
     **********/
    data  = binDataCopy[ 1 ] << 4 ; // 3210xxxx
    data |= binDataCopy[ 2 ] >> 4 ; // xxxx7654
    data &= 0x1F                  ; // ...07654
    base32Data[ 3 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 2 - bits 3 ~ 0
     * Byte 3 - bit 7
     *
     * | Byte 2                        | Byte 3
     * | 7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *                  \________ ________/
     *                           |
     *                           +-----> Data 4
     *
     **********/
    data  = binDataCopy[ 2 ] << 1 ; // 6543210x
    data |= binDataCopy[ 3 ] >> 7 ; // xxxxxxx7
    data &= 0x1F                  ; // ...32107
    base32Data[ 4 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 3 - bits 6 ~ 2
     *
     * | Byte 3
     * | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+
     *      \________ ________/
     *               |
     *               +-----> Data 5
     *
     **********/
    data  = binDataCopy[ 3 ] >> 2 ; // xx765432
    data &= 0x1F                  ; // ...65432
    base32Data[ 5 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 3 - bits 1 ~ 0
     * Byte 4 - bits 7 ~ 5
     *
     * | Byte 3                        | Byte 4
     * | 7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *                          \________ ________/
     *                                   |
     *                                   +-----> Data 6
     *
     **********/
    data  = binDataCopy[ 3 ] << 3 ; // 43210xxx
    data |= binDataCopy[ 4 ] >> 5 ; // xxxxx765
    data &= 0x1F                  ; // ...10765
    base32Data[ 6 ] = base32_table[ data ] ;

    /**********
     *
     * Byte 4 - bits 4 ~ 0
     *
     * | Byte 4
     * | 7   6   5   4   3   2   1   0
     * +---+---+---+---+---+---+---+---+
     * |   |   |   |   |   |   |   |   |
     * +---+---+---+---+---+---+---+---+
     *              \________ ________/
     *                       |
     *                       +-----> Data 7
     *
     **********/
    data  = binDataCopy[ 4 ]      ; // 76543210
    data &= 0x1F                  ; // ...43210
    base32Data[ 7 ] = base32_table[ data ] ;
    
    switch( binDataCopySize )
    {
    case 1 : // 1Byte -> 6 paddings.
        memset( base32Data + 2 , '=' , 6 ) ;
        break ;
    case 2 : // 2Byte -> 4 paddings.
        memset( base32Data + 4 , '=' , 4 ) ;
        break ;
    case 3 : // 3Byte -> 3 paddings.
        memset( base32Data + 5 , '=' , 3 ) ;
        break ;
    case 4 : // 4Byte -> 1 paddings.
        memset( base32Data + 7 , '=' , 1 ) ;
        break ;
    }
}
