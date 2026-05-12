/**
 * @file binSplit.c
 * @author Francesco Sacco
 * @date 05 May 2026
 * @brief This project split binary files.
 *
 * This software is a tool to cut part of the binary files
 * into a small parts.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-05-06 - 0.0.0 - Initial version.
  *
  **********/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "utils.h"

#define BUFFER_SIZE                              4096

int getFileSize( FILE * in , size_t * sizeOut ) ;

int main( int argc , char ** argv )
{
    FILE * fileIn = NULL ;
    FILE * fileOut = NULL;

    long start;
    long size;
    size_t file_size;
    int getSizeRet ;

    unsigned char buffer[BUFFER_SIZE];

    // Initial messages.
    printf( "Binary Split - Version 0.0.0\n" ) ;
    printf( "github.com/francescosacco/BinaryTools\n" ) ;

    if( argc != 5 )
    {
        fprintf( stderr , "\tUsage: %s <input.bin> <output.bin> <start> <size>\n" , argv[ 0 ] ) ;
        return 0 ;
    }

    // Arguments parser.
    if( !parse_number( argv[ 3 ] , &start ) )
    {
        fprintf( stderr , "Invalid start offset: %s\n" , argv[ 3 ] ) ;
        return 1 ;
    }

    if( !parse_number( argv[ 4 ] , &size ) )
    {
        fprintf( stderr , "Invalid size: %s\n" , argv[ 4 ] ) ;
        return 1 ;
    }

    if( ( start < 0 )  || ( size < 0 ) )
    {
        fprintf( stderr , "\tInvalid start or size value.\n" ) ;
        return 1 ;
    }

    // Open input file.
    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( !fileIn )
    {
        perror( "\tError" ) ;
        return errno ;
    }

    // Create output file.
    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( !fileOut )
    {
        int errno_fopen = errno ;

        perror( "\tError" ) ;
        fclose( fileIn ) ;

        return errno_fopen ;
    }

    // Get input file size.
    getSizeRet = getFileSize( fileIn , &file_size ) ;
    if( getSizeRet )
    {
        fprintf( stderr , "\tError checking file size.\n" ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;
        return getSizeRet ;
    }
    printf( "\tThe size of %s is %zu bytes.\n" , argv[ 1 ] , file_size ) ;

    // Validations.
    if( start >= file_size )
    {
        fprintf( stderr , "\tThe address is out of the file.\n" ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;
        return 1 ;
    }

    if( ( start + size ) > file_size )
    {
        fprintf( stderr , "\tRequested range is out of the file.\n" ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;
        return 1 ;
    }

    // Set initial position in the file.
    if( fseek( fileIn , start , SEEK_SET ) != 0 )
    {
        fprintf( stderr , "\tError seeking input file.\n" ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;
        return 1 ;
    }

    // Copy data.
    long remaining = size ;
    printf( "\tCopy %lu bytes.\n" , size ) ;

    while( remaining > 0 )
    {
        size_t chunk = ( remaining > BUFFER_SIZE ) ? BUFFER_SIZE : ( size_t ) remaining ;
        size_t read_bytes = fread( buffer , sizeof( buffer[ 0 ] ) , chunk , fileIn ) ;

        if( read_bytes != chunk )
        {
            fprintf( stderr , "\tError reading input file.\n" ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return 1 ;
        }

        if( fwrite( buffer , sizeof( buffer[ 0 ] ) , read_bytes , fileOut ) != read_bytes )
        {
            fprintf( stderr, "\tError writing output file.\n" ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return 1 ;
        }

        remaining -= read_bytes ;
    }

    fclose( fileIn ) ;
    fclose( fileOut ) ;

    return 0 ;
}

int getFileSize( FILE * in , size_t * sizeOut )
{
    int ret ;
    fpos_t pos ;

    // Save the current position.
    ret = fgetpos( in , &pos ) ;
    if( ret )
    {
        perror( "\tError" ) ;
        return( ret ) ;
    }

    // Set position to END OF FILE.
    ret = fseek( in , 0 , SEEK_END ) ;
    if( ret )
    {
        perror( "\tError" ) ;
        return( ret ) ;
    }

    // Get current position.
    long ftell_ret ;
    ftell_ret = ftell( in ) ;
    if( ftell_ret < 0 )
    {
        perror( "\tError" ) ;
        return( errno ) ;
    }

    // Load size to output parameter.
    *sizeOut = ( size_t ) ftell_ret ;

    // Restore the original position.
    ret = fsetpos( in , &pos ) ;

    return ret ;
}
