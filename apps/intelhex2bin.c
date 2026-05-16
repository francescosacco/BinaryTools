/**
 * @file intelhex2bin.c
 * @author Francesco Sacco
 * @date 11 May 2026
 * @brief This project convert Intel HEX into binary.
 *
 * This software is a tool convert Intel Hex files into Binary files.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-05-15 - 0.0.0 - Initial version.
  *
  **********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "utils.h"
#include "hex.h"

int main( int argc , char ** argv )
{
    FILE * fileIn = NULL ;
    FILE * fileOut = NULL ;

    unsigned char fill = 0x00 ;
    int strict = 0 ;

    // Initial messages.
    printf( "Intel Hex to Binary - Version 0.0.0\n" ) ;
    printf( "github.com/francescosacco/BinaryTools\n" ) ;

    if( argc < 3 )
    {
        fprintf( stderr , "\tUsage: %s <input.hex> <output.bin> [--fill <value>] [--strict]\n" , argv[ 0 ] ) ;
        return 1 ;
    }

    // Parser optional arguments.
    for( int i = 3 ; i < argc ; i++ )
    {
        if( strcmp( argv[ i ] , "--fill" ) == 0 )
        {
            if( ( i + 1 ) >= argc )
            {
                fprintf(stderr, "\tMissing value for --fill.\n" ) ;
                return 1 ;
            }

            long val ;
            if( !parse_number( argv[ i + 1 ] , &val ) || ( val > 0xFF ) )
            {
                fprintf( stderr , "\tInvalid fill value.\n" ) ;
                return 1 ;
            }

            fill = ( unsigned char ) val ;
            i++ ;
        }
        else if( strcmp( argv[ i ] , "--strict" ) == 0 )
        {
            strict = 1 ;
        }
        else
        {
            fprintf( stderr , "\tUnknown argument: %s\n" , argv[ i ] ) ;
            return 1 ;
        }
    }

    // Open input file.
    fileIn = fopen( argv[ 1 ] , "r" ) ;
    if( !fileIn )
    {
        fprintf( stderr , "\tError opening input: %s\n" , strerror( errno ) ) ;
        return 1 ;
    }

    // Allocate dynamic memory for this task.
    size_t capacity = 1024 ;
    uint8_t * memory  = malloc( capacity ) ;
    uint8_t * written = malloc( capacity ) ;
    char    * line    = malloc( capacity ) ;

    if( ( memory == NULL ) || ( written == NULL ) || ( line == NULL ) )
    {
        fprintf( stderr , "\tMemory allocation failed.\n" ) ;
        return 1 ;
    }

    memset( memory  , fill , capacity ) ;
    memset( written , 0    , capacity ) ;
    memset( line    , '\0' , capacity ) ;

    unsigned int base_addr = 0 ;
    unsigned int max_addr = 0 ;
    int line_number = 0 ;

    while( fgets( line , 1024 , fileIn ) )
    {
        line_number++ ;

        if( line[ 0 ] != ':' )
        {
            fprintf( stderr , "\tLine %d: invalid format\n" , line_number ) ;
            goto error ;
        }

        int len     = hex_byte( line + 1 ) ;
        int addr_hi = hex_byte( line + 3 ) ;
        int addr_lo = hex_byte( line + 5 ) ;
        int type    = hex_byte( line + 7 ) ;

        if( ( len < 0 ) || ( addr_hi < 0 ) || ( addr_lo < 0 ) || ( type < 0 ) )
        {
            fprintf( stderr , "\tLine %d: parse error.\n" , line_number ) ;
            goto error ;
        }

        // Check the checksum.
        int total_len = 9 + ( len * 2 ) + 2;
        if( !validate_checksum( line + 1 , total_len - 1 ) )
        {
            fprintf( stderr , "\tLine %d: checksum error.\n" , line_number ) ;
            goto error ;
        }

        unsigned int addr = ( addr_hi << 8 ) | addr_lo ;
        const char * data = line + 9 ;

        if( type == 0x00 )
        {
            unsigned int abs_addr = base_addr + addr ;

            // Resize memory if necessary.
            while( ( abs_addr + len ) > capacity )
            {
                size_t new_cap = capacity * 2 ;
                memory = realloc( memory , new_cap ) ;
                written = realloc( written , new_cap ) ;

                if( ( memory == NULL ) || ( written == NULL ) )
                {
                    goto error ;
                }

                memset( memory  + capacity , fill , new_cap - capacity ) ;
                memset( written + capacity , 0    , new_cap - capacity ) ;

                capacity = new_cap ;
            }

            for( int i = 0 ; i < len ; i++ )
            {
                int val = hex_byte( data + ( i * 2 ) ) ;
                if( val < 0 )
                {
                    fprintf( stderr , "\tLine %d: invalid data\n" , line_number ) ;
                    goto error ;
                }

                unsigned int pos = ( abs_addr + i ) ;

                if( written[ pos ] && ( strict ) )
                {
                    fprintf( stderr , "\tLine %d: overlapping data at 0x%X\n" , line_number , pos ) ;
                    goto error ;
                }

                memory[  pos ] = ( uint8_t ) val ;
                written[ pos ] = 1 ;
            }

            if( ( abs_addr + len ) > max_addr )
            {
                max_addr = abs_addr + len ;
            }
        }
        else if( type == 0x04 )
        {
            int msb = hex_byte( data     ) ;
            int lsb = hex_byte( data + 2 ) ;

            base_addr = ( ( msb << 8 ) | lsb ) << 16 ;
        }
        else if( type == 0x02 )
        {
            int msb = hex_byte( data     ) ;
            int lsb = hex_byte( data + 2 ) ;

            base_addr = ( ( msb << 8 ) | lsb ) << 4 ;
        }
        else if( type == 0x01 )
        {
            break ;
        }
    }

    // Write data in the output file.
    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( !fileOut )
    {
        fprintf( stderr , "\tError opening output\n" ) ;
        goto error ;
    }

    fwrite( memory , sizeof( memory[ 0 ] ) , max_addr , fileOut ) ;

    // Report.
    uint8_t firstDataFound = 0 ;
    uint32_t firstDataAddr = 0 ;
    uint32_t usefulDataSize = 0 ;
    for( unsigned int i = 0 ; i < max_addr ; i++ )
    {
        // Find first data.
        if( ( firstDataFound == 0 ) && ( written[ i ] != 0 ) )
        {
            firstDataAddr = ( uint32_t ) i ;
            firstDataFound = 1 ;
        }

        // Calculate useful data.
        if( written[ i ] != 0 )
        {
            usefulDataSize++ ;
        }
    }
    
    // Print report.
    printf( "\tFirst data in 0x%08X.\n" , firstDataAddr ) ;
    printf( "\tOutput file size %u.\n" , max_addr ) ;
    printf( "\tOutput useful size %u.\n" , usefulDataSize ) ;

    fclose( fileIn ) ;
    fclose( fileOut ) ;
    free( memory ) ;
    free( written ) ;
    free( line ) ;

    return 0 ;

error:
    fclose( fileIn ) ;
    if( fileOut )
    {
        fclose( fileOut ) ;
    }
    free( memory ) ;
    free( written ) ;
    free( line ) ;
    return 1 ;
}
