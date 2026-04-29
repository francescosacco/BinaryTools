/**
 * @file hex.c
 * @author Francesco Sacco
 * @date 29 Apr 2026
 * @brief This library manage the conversion between HEX and BIN.
 *
 * This library was developed to manage the conversion between binary data
 * and hexadecimal data.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

#include "hex.h"

static const char hex_table[] = "0123456789ABCDEF";

size_t bin_to_hex(const uint8_t * input , size_t input_len , char * output )
{
    size_t i , j ;

    for( i = 0 , j = 0 ; i < input_len ; i++ , j += 2 )
    {
        uint8_t lsn =   input[ i ]        & 0x0F ; // Least Significant Nibble.
        uint8_t msn = ( input[ i ] >> 4 ) & 0x0F ; // Most Significant nibble.
        
        output[ j    ] = hex_table[ msn ] ;
        output[ j + 1] = hex_table[ lsn ] ;
    }

    return j ;
}
