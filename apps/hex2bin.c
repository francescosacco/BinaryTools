/**
 * @file hex2bin.h
 * @author Francesco Sacco
 * @date 08 Feb 2018
 * @brief This project converts HEX files to binary files.
 *
 * This software is a tool to convert HEX files to binary string
 * files.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-04-28 - 1.0.0 - Architecture review.
  * 2019-03-01 - 0.1.2 - Fixed new library.
  * 2019-02-12 - 0.1.1 - Change exhibition of file name.
  * 2019-02-11 - 0.1.0 - Created library with hex2bin code.
  * 2019-02-09 - 0.0.1 - Fix upper case.
  * 2019-02-08 - 0.0.0 - Initial version.
  *
  **********/
 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "hex.h"

#define IN_BUF_SIZE                              4096
#define OUT_BUF_SIZE                             ( IN_BUF_SIZE / 2 )

static void print_hex_error(hex_status_t status)
{
    switch (status) {
    case HEX_ERR_INVALID_CHAR:
        fprintf(stderr, "Invalid hex character found\n");
        break;
    case HEX_ERR_ODD_LENGTH:
        fprintf(stderr, "Odd number of hex digits\n");
        break;
    default:
        break;
    }
}

int main( int argc , char ** argv )
{
    FILE * fileIn ;
    FILE * fileOut ;

    char inbuf[ IN_BUF_SIZE ] ;
    unsigned char outbuf[ OUT_BUF_SIZE ] ;
    
    size_t total_bytes_read ;
    size_t total_bytes_written ;
    

    // Initial messages.
    printf( "Hex To Bin - Version 1.0.0\n" ) ;
    printf( "github.com/francescosacco/BinaryTools\n" ) ;

    if( argc != 3 )
    {
        fprintf( stderr , "\tUsage: %s <input.hex> <output.bin>\n" , argv[ 0 ] ) ;
        return 0 ;
    }

    fileIn = fopen( argv[ 1 ] , "r" ) ;
    if( !fileIn )
    {
        perror( "\tError" ) ;
        return errno ;
    }

    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( !fileOut )
    {
        int errno_fopen = errno ;

        perror( "\tError" ) ;
        fclose( fileIn ) ;

        return errno_fopen ;
    }

    total_bytes_read = 0 ;
    total_bytes_written = 0 ;

    while( !feof( fileIn ) )
    {
        size_t bytes_read = fread( inbuf , sizeof( inbuf[ 0 ] ) , sizeof( inbuf ) , fileIn ) ;
        total_bytes_read += bytes_read ;
        if( bytes_read == 0 )
        {
            break ;
        }

        size_t bytes_written = 0 ;
        hex_status_t status = hex_to_bin( inbuf , bytes_read , outbuf , &bytes_written ) ;
        if( status != HEX_OK )
        {
            print_hex_error( status ) ;
            fclose( fileIn  ) ;
            fclose( fileOut ) ;
            return 1 ;
        }

        if( fwrite( outbuf , sizeof( outbuf[ 0 ] ) , bytes_written , fileOut ) != bytes_written )
        {
            int errno_fopen = errno ;

            perror( "\tError" ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;

            return errno_fopen ;
        }

        total_bytes_written += bytes_written ;
    }

    printf( "\tTotal size read %zu bytes\n" , total_bytes_read ) ;
    printf( "\tTotal size written %zu bytes\n" , total_bytes_written ) ;

    fclose( fileIn ) ;
    fclose( fileOut ) ;
    return 0 ;
}
