/**
 * @file bin2hex.h
 * @author Francesco Sacco
 * @date 29 May 2018
 * @brief This project converts binary files to HEX files.
 *
 * This software is a tool to convert binary files to HEX string
 * files.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-04-28 - 1.0.0 - Architecture review.
  * 2018-12-26 - 0.2.3 - Fix fpos_t access for linux compilation.
  * 2018-12-11 - 0.2.2 - Fix file error.
  * 2018-12-06 - 0.2.1 - Fix argv access.
  * 2018-12-06 - 0.2.0 - Add possibility to print at stdout.
  * 2018-05-29 - 0.1.0 - Add columns control.
  * 2018-05-25 - 0.0.1 - Fix data type to print at console.
  * 2018-05-18 - 0.0.0 - Initial version.
  *
  **********/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "hex.h"

#define BUFFER_SIZE                              4096u

int main(int argc , char ** argv )
{
    FILE * fileIn ;
    FILE * fileOut ;

    unsigned char bin_buf[ BUFFER_SIZE ] ;
    char hex_buf[ BUFFER_SIZE * 2 ] ;

    size_t bytes_read ;
    size_t bytes_written ;
    size_t total_bytes_written ;
    size_t total_bytes_read ;

    // Initial messages.
    printf( "Bin To Hex - Version 1.0.0\n" ) ;
    printf( "github.com/francescosacco/BinaryTools\n" ) ;

    if( argc != 3 )
    {
        fprintf( stderr , "\tUsage: %s <input.bin> <output.hex>\n" , argv[ 0 ] ) ;
        return 0 ;
    }

    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( !fileIn )
    {
        perror( "\tError" ) ;
        return errno ;
    }

    fileOut = fopen( argv[ 2 ] , "w" ) ;
    if( !fileOut )
    {
        int errno_fopen = errno ;

        perror( "\tError" ) ;
        fclose( fileIn ) ;

        return errno_fopen ;
    }

    total_bytes_written = 0 ;
    total_bytes_read = 0 ;

    while( ( bytes_read = fread( bin_buf , sizeof( bin_buf[ 0 ] ) , sizeof( bin_buf ) , fileIn ) ) > 0 )
    {
        bytes_written = bin_to_hex( bin_buf , bytes_read , hex_buf ) ;

        total_bytes_read += bytes_read ;
        total_bytes_written += bytes_written ;

        if( fwrite( hex_buf , sizeof( hex_buf[ 0 ] ) , bytes_written , fileOut ) != bytes_written )
        {
            int errno_fopen = errno ;

            perror( "\tError" ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;

            return errno_fopen ;
        }
    }

    if( ferror( fileIn ) )
    {
        perror( "\tError" ) ;
    }

    printf( "\tTotal size read %zu bytes\n" , total_bytes_read ) ;
    printf( "\tTotal size written %zu bytes\n" , total_bytes_written ) ;

    fclose( fileIn ) ;
    fclose( fileOut ) ;

    return 0 ;
}
