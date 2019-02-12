/**
 * @file bin2base64.c
 * @author Francesco Sacco
 * @date 10 Dec 2018
 * @brief This project converts binary files to Base-64 files.
 *
 * This software is a tool to convert binary files to Base-64
 * files.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-12 - 0.1.0 - Created library with Base-64 code.
  * 2018-12-10 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../library/base64.h"

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    fpos_t  fileInSize , i , j ;
    int chr , ret ;
    
    uint8_t bufferBin[ 4 ] ;
    uint8_t buffer64[ 5 ] ;
    
    // Initial messages.
    printf( "Bin To Base64 - Version 0.1.0\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 3 )
    {
        printf( "Usage: bin2base64 <Binary File In> <Base64 File Out>\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }
    
    // Open Binary File.
    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }

    // Create Base64 File.
    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( fileOut == ( ( FILE * ) NULL ) )
    {
        fclose( fileIn ) ;
        printf( "\tError to create \"%s\"\n" , argv[ 2 ] ) ;
        return( -1 ) ;
    }
    
    // Check binary file size.
    fileInSize = 0 ;
    ret = fseek( fileIn , 0 , SEEK_END ) ;
    ret = ( ret == 0 ) ? fgetpos( fileIn , &fileInSize ) : ( ret ) ;
    ret = ( ret == 0 ) ? fseek( fileIn , 0 , SEEK_SET ) : ( ret ) ;
    if( ret != 0 )
    {
        ret = ferror( fileIn ) ;
        printf( "\tError to get binary file size!\n" ) ;
        printf( "\tferror [ %d ]\n" , ret ) ;
        fclose( fileIn ) ;
        fclose( fileOut ) ;
        return( ret ) ;
    }
    printf( "\t\"%s\" - Size = %lu\n" , argv[ 1 ] , ( unsigned long ) fileInSize ) ;

    i = 0 ;
    while( i < fileInSize )
    {
        for( j = 0 ; ( j < 3 ) && ( i < fileInSize ) ; j++, i++ )
        {
            chr = getc( fileIn ) ;
            if( chr == EOF )
            {
                ret = ferror( fileIn ) ;
                printf( "\tError to read binary file at %lu!\n" , ( unsigned long ) i ) ;
                printf( "\tferror [ %d ]\n" , ret ) ;
                fclose( fileIn ) ;
                fclose( fileOut ) ;
                return( ret ) ;
            }

            bufferBin[ j ] = ( uint8_t ) chr ;
        }

        bin2base64( bufferBin , j , buffer64 ) ;
        
        ret = fprintf( fileOut , "%s" , buffer64 ) ;
        if( ret < 0 )
        {
            ret = ferror( fileOut ) ;
            printf( "\tError to write base64 file!\n" ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return( ret ) ;
        }
    }
    
    printf( "\tDone!\n" ) ;
    
    return( 0 ) ;
}
