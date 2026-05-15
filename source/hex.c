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

#include <stdio.h>

static int hex_value( char c ) ;

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

hex_status_t hex_to_bin( const char * input , size_t input_len , unsigned char * output , size_t * output_len)
{
    size_t out_idx = 0 ;
    int high_nibble = -1 ;

    for( size_t i = 0 ; i < input_len ; i++ )
    {
        char c = input[ i ] ;

        // Ignore line breaks.
        if( ( c == '\n' ) || ( c == '\r' ) )
        {
            continue ;
        }

        int value = hex_value( c ) ;
        if( value < 0 )
        {
            return HEX_ERR_INVALID_CHAR ;
        }

        if( high_nibble < 0 )
        {
            high_nibble = value ;
        }
        else
        {
            output[ out_idx++ ] = ( high_nibble << 4 ) | value ;
            high_nibble = -1 ;
        }
    }

    // Odd number of digits.
    if( high_nibble >= 0 )
    {
        return HEX_ERR_ODD_LENGTH ;
    }

    if( output_len )
    {
        *output_len = out_idx ;
    }

    return HEX_OK ;
}

static int hex_value( char c )
{
    int value = -1 ;
    
    if( ( c >= '0' ) && ( c <= '9') )
    {
        value = ( int ) ( c - '0' ) ;
    }
    else if( ( c >= 'A' ) && ( c <= 'F' ) )
    {
        value = ( int ) ( c - 'A' + 0x0A ) ;
    }
    else if( ( c >= 'a' ) && ( c <= 'f' ) )
    {
        value = ( int ) ( c - 'a' + 0x0A ) ;
    }

    return value ;
}


static unsigned char checksum( const unsigned char * data , int len )
{
    unsigned int sum = 0 ;
    unsigned char ret ;

    for( int i = 0 ; i < len ; i++ )
    {
        sum += data[ i ] ;
    }

    ret = ( unsigned char ) ( ( ~sum + 1 ) & 0xFF ) ;

    return ret ;
}

size_t ihex_write_record( char * output , unsigned char len , unsigned short addr , unsigned char type , const unsigned char * data )
{
    unsigned char buffer[ 5 + 256 ] ;
    int idx = 0 ;
    int out_idx = 0 ;

    buffer[ idx++ ] = len ;
    buffer[ idx++ ] = ( addr >> 8 ) & 0xFF ;
    buffer[ idx++ ] = addr & 0xFF ;
    buffer[ idx++ ] = type ;

    for( int i = 0 ; i < len ; i++ )
    {
        buffer[ idx++ ] = data[ i ] ;
    }

    unsigned char cs = checksum( buffer , idx ) ;

    out_idx += sprintf( output + out_idx , ":" ) ;

    for( int i = 0 ; i < idx ; i++ )
    {
        out_idx += sprintf( output + out_idx , "%02X" , buffer[ i ] ) ;
    }

    out_idx += sprintf( output + out_idx , "%02X\n" , cs ) ;

    return out_idx ;
}

int hex_byte( const char * s )
{
    int hi = hex_value( s[ 0 ] ) ;
    int lo = hex_value( s[ 1 ] ) ;

    if( ( hi < 0 ) || ( lo < 0 ) )
    {
        return -1 ;
    }

    return ( hi << 4 ) | lo ;
}

int validate_checksum( const char * line , int len )
{
    int sum = 0 ;

    for( int i = 0 ; i < len ; i += 2 )
    {
        int val = hex_byte( line + i ) ;
        if( val < 0 )
        {
            return 0 ;
        }

        sum += val ;
    }

    return ( sum & 0xFF ) == 0 ;
}
