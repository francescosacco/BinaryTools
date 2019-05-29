/**
 * @file binText.h
 * @author Francesco Sacco
 * @date 27 May 2019
 * @brief This project prints all text integrated in a binary file.
 *
 * This program identify and show all text integrated into a binary
 * file.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-05-28 - 0.0.1 - Fixes segmentation fault.
  * 2019-05-27 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h> // To use strstr().
#include <stdlib.h> // To use atoi().
#include <stdint.h>

uint32_t getFileSize( FILE * in ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    uint32_t fileInSize , i ;
    int chr , ret ;
    
    char bufText[ 256 ] ;
    uint16_t bufTextSize ;
    uint32_t bufAddr ;
    char printIt = 0 ;
    
    // Initial messages.
    printf( "Bin Text - Version 0.0.1\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 2 )
    {
        printf( "Usage: binText <Binary File In>\n" ) ;
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

    // Check binary file size.
    fileInSize = getFileSize( fileIn ) ;
    
    for( i = 0 , bufTextSize = 0 ; i < fileInSize ; i++ )
    {
        chr = getc( fileIn ) ;
        if( chr == EOF )
        {
            ret = ferror( fileIn ) ;
            printf( "\tError to read binary file at %lu!\n" , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            return( ret ) ;
        }    
     
        if( ( chr >= 0x20 ) && ( chr <= 0x7E ) )
        {
            if( bufTextSize == 0 )
            {
                memset( bufText , '\0' , sizeof( bufText ) ) ;
                bufAddr = i ;
            }
            
            bufText[ bufTextSize++ ] = ( char ) chr ;
        }
        else
        {
            if( bufTextSize )
            {
                printIt = 1 ;
            }
        }
        
        if( ( printIt ) || ( bufTextSize >= 256 ) )
        {
            uint32_t j , colCount ;
            
            printf( "%04X.%04Xh - \"" , ( uint16_t ) ( bufAddr >> 16 ) , ( uint16_t ) ( bufAddr & 0x0000FFFF ) ) ;
            for( j = 0 , colCount = 0 ; j < bufTextSize ; j++ )
            {
                if( bufText[ j ] != '\n' )
                {
                    printf( "%c" , bufText[ j ] ) ;
                    colCount++ ;
                }
                else if( bufText[ j ] != '\r' )
                {
                    // Just ignore.
                }
                else
                {
                    for( ; colCount < 48 ; colCount++ )
                    {
                        printf( " " ) ;
                    }
                }
                
                if( colCount >= 48 )
                {
                    printf( "\"\n" ) ;
                    printf( "             \"" ) ;
                    colCount = 0 ;
                }
            }

            printf( "\"\n" ) ;

            bufTextSize = 0 ;
            printIt = 0 ;
        }
    }
    
    fclose( fileIn ) ;
    
    return( 0 ) ;
}

uint32_t getFileSize( FILE * in )
{
    uint32_t ret = 0 ;
    fpos_t pos ;
    int fPosRet ;
    
    fPosRet = fgetpos( in , &pos ) ;
    if( fPosRet )
    {
        return( ret ) ;
    }
    
    fPosRet = fseek( in , 0 , SEEK_END ) ;
    if( fPosRet )
    {
        return( ret ) ;
    }
    
    ret = ( uint32_t ) ftell( in ) ;
    if( ret == 0xFFFFFFFF )
    {
        ret = 0 ;
        return( ret ) ;
    }

    ( void ) fsetpos( in , &pos ) ;
    
    return( ret ) ;
}
