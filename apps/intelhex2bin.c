#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "utils.h"
#include "hex.h"

int main( int argc , char ** argv )
{
    FILE * fin = NULL ;
    FILE * fout = NULL ;

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
    fin = fopen( argv[ 1 ] , "r" ) ;
    if( !fin )
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

    while( fgets( line , 1024 , fin ) )
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
    fout = fopen( argv[ 2 ] , "wb" ) ;
    if( !fout )
    {
        fprintf( stderr , "\tError opening output\n" ) ;
        goto error ;
    }

    fwrite( memory , sizeof( memory[ 0 ] ) , max_addr , fout ) ;

    fclose( fin ) ;
    fclose( fout ) ;
    free( memory ) ;
    free( written ) ;
    free( line ) ;

    return 0 ;

error:
    fclose( fin ) ;
    if( fout )
    {
        fclose( fout ) ;
    }
    free( memory ) ;
    free( written ) ;
    free( line ) ;
    return 1 ;
}
