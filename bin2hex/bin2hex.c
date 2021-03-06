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
#include <string.h> // To use strstr().
#include <stdlib.h> // To use atoi().
#include <stdint.h>

uint32_t getFileSize( FILE * in ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    char * fileInName = NULL ;
    char * fileOutName = NULL ;
    uint32_t fileInSize , fileOutSize , i ;
    int chr , ret , colCount , colSize = 0 ;
    
    // Initial messages.
    printf( "Bin To Hex - Version 0.2.2\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 2 )
    {
        printf( "Usage: bin2hex <Binary File In> [Hex File Out] [-c50]\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }
    else if( argc == 2 )
    {
        fileInName = argv[ 1 ] ;
    }
    else if( argc == 3 )
    {
        char * str = strstr( argv[ 2 ] , "-c" ) ;
	if( str )
	{
            colSize = atoi( &argv[ 2 ][ 2 ] ) ;
	}
	else
	{
            fileOutName = argv[ 2 ] ;
	}

	fileInName = argv[ 1 ] ;
    }
    else
    {
        char * str = strstr( argv[ 3 ] , "-c" ) ;

        fileInName  = argv[ 1 ] ;
	fileOutName = argv[ 2 ] ;
	if( str )
	{
            colSize = atoi( &argv[ 3 ][ 2 ] ) ;
	}
	
    }

    if( colSize > 0 )
    {
        printf( "\tConfigured %d columns.\n" , colSize ) ;
    }
    
    // Open Binary File.
    fileIn = fopen( fileInName , "rb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , fileInName ) ;
        return( -1 ) ;
    }

    // Create Hex File.
    if( fileOutName )
    {
        fileOut = fopen( fileOutName , "wb" ) ;
        if( fileOut == ( ( FILE * ) NULL ) )
        {
            fclose( fileIn ) ;
            printf( "\tError to create \"%s\"\n" , fileOutName ) ;
            return( -1 ) ;
        }
    }
    else
    {
        fileOut = stdout ;
    }
    
    // Check binary file size.
    fileInSize = getFileSize( fileIn ) ;
    printf( "\t\"%s\" - Size = %lu\n" , fileInName , ( unsigned long ) fileInSize ) ;
    
    for( i = 0 , fileOutSize = 0 , colCount = 0 ; i < fileInSize ; i++ )
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
        
	ret = fprintf( fileOut , "%02X" , ( unsigned char ) ( chr ) ) ;
        if( ret < 0 )
        {
            ret = ferror( fileOut ) ;
            printf( "\tError to write hex file!\n" ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return( ret ) ;
        }
        
        colCount += 2 ;
        if( ( colSize ) && ( colCount >= colSize ) )
        {
          ( void ) fprintf( fileOut , "\n" ) ;
          colCount = 0 ;
        }
        
        fileOutSize += ( uint32_t ) ret ;
    }
    
    if( fileOutName )
    {
        printf( "\t\"%s\" - Size = %lu\n" , fileOutName , ( unsigned long ) fileOutSize ) ;
	printf( "\tDone!\n" ) ;
    }
    
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
