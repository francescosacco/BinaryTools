/**
 * @file bin2intelhex.c
 * @author Francesco Sacco
 * @date 11 May 2026
 * @brief This project convert binary files into Intel HEX.
 *
 * This software is a tool convert binary files into Intel Hex format.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-05-11 - 0.0.0 - Initial version.
  *
  **********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "hex.h"
#include "utils.h"

#define DATA_SIZE 16

int getFileSize( FILE * in , size_t * sizeOut ) ;

int main( int argc , char ** argv )
{
    FILE * fileIn = NULL ;
    FILE * fileOut = NULL ;

    ihex_mode_t mode = IHEX_MODE_LINEAR ;
    unsigned int offset = 0 ;

    // Check the minimum arguments.
    if( argc < 3 )
    {
        fprintf( stderr , "\tUsage: %s <input.bin> <output.hex> [--segment|--linear] [--offset <value>]\n" , argv[ 0 ] ) ;
        return 1 ;
    }

    // Parser optional arguments.
    for( int i = 3 ; i < argc ; i++ )
    {
        if( strcmp( argv[ i ] , "--segment" ) == 0 )
        {
            mode = IHEX_MODE_SEGMENT ;
        }
        else if( strcmp( argv[ i ] , "--linear" ) == 0 )
        {
            mode = IHEX_MODE_LINEAR ;
        }
        else if( strcmp( argv[ i ] , "--offset" ) == 0 )
        {
            if( ( i + 1 ) >= argc )
            {
                fprintf( stderr , "\tMissing value for --offset\n" ) ;
                return 1 ;
            }

            long val ;
            if( !parse_number( argv[ i + 1 ] , &val ) )
            {
                fprintf( stderr , "\tInvalid offset: %s\n" , argv[ i + 1 ] ) ;
                return 1 ;
            }

            offset = ( unsigned int ) val ;
            i++ ; /* consumir argumento */
        }
        else
        {
            fprintf( stderr , "\tUnknown argument: %s\n" , argv[ i ] ) ;
            return 1 ;
        }
    }

    // Open input file.
    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( !fileIn )
    {
        perror( "\tError" ) ;
        return errno ;
    }

    // Create output file.
    fileOut = fopen( argv[ 2 ] , "w" ) ;
    if( !fileOut )
    {
        int errno_fopen = errno ;

        perror( "\tError" ) ;
        fclose( fileIn ) ;

        return errno_fopen ;
    }

    // Get input file size.
    size_t file_size = 0 ;
    int fileSizeRet = getFileSize( fileIn , &file_size ) ;
    if( fileSizeRet )
    {
        fprintf( stderr , "\tError checking file size.\n" ) ;
        fclose(fileIn);
        fclose(fileOut);
        return fileSizeRet ;
    }

    // Check file size limit.
    if( ( file_size + ( size_t ) offset ) > ( size_t ) 0x100000000ULL )
    {
        fprintf( stderr , "\tError: address space exceeds 4GB limit.\n" ) ;
        fclose(fileIn);
        fclose(fileOut);
        return 1;
    }

    // Conversion.
    unsigned char buffer[ DATA_SIZE ] ;
    char line[ 600 ] ;

    unsigned int address = offset ;
    unsigned int current_ext = 0xFFFFFFFF ;

    for( ; /* EVER */ ; )
    {
        size_t n = fread( buffer , sizeof( buffer[ 0 ] ) , DATA_SIZE , fileIn ) ;
        if( n == 0 )
        {
            break ;
        }

        unsigned int ext ;

        if( mode == IHEX_MODE_LINEAR )
        {
            ext = address >> 16 ;
        }
        else
        {
            ext = address >> 4 ;
        }

        // Update extended address.
        if( ext != current_ext )
        {
            unsigned char ext_data[ 2 ] ;
            ext_data[ 0 ] = ( ext >> 8 ) & 0xFF ;
            ext_data[ 1 ] =   ext & 0xFF ;

            size_t len ;

            if( mode == IHEX_MODE_LINEAR )
            {
                len = ihex_write_record( line , 2 , 0 , 0x04 , ext_data ) ;
            }
            else
            {
                len = ihex_write_record( line , 2 , 0 , 0x02 , ext_data ) ;
            }

            if( fwrite( line , sizeof( line[ 0 ] ) , len , fileOut ) != len )
            {
                int errno_fopen = errno ;

                perror( "\tError" ) ;
                fclose( fileIn ) ;
                fclose( fileOut ) ;

                return errno_fopen ;
            }

            current_ext = ext ;
        }

        // Write record.
        size_t len = ihex_write_record( line , ( unsigned char ) n , address & 0xFFFF , 0x00 , buffer ) ;
        if( fwrite( line , sizeof( line[ 0 ] ) , len , fileOut ) != len )
        {
            int errno_fopen = errno ;

            perror( "\tError" ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;

            return errno_fopen ;
        }

        address += n ;
    }

    // Write End of File.
    size_t len = ihex_write_record( line , 0 , 0 , 0x01 , NULL ) ;
    if( fwrite( line , sizeof( line[ 0 ] ) , len , fileOut ) != len )
    {
        int errno_fopen = errno ;

        perror( "\tError" ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;

        return errno_fopen ;
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
