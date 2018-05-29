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
  * 2018-05-29 - 0.1.0 - Add columns control.
  * 2018-05-25 - 0.0.1 - Fix data type to print at console.
  * 2018-05-18 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h> // To use strstr().
#include <stdlib.h> // To use atoi().

int main( int argc , char * argv[] )
{
    FILE * fileIn  ;
    FILE * fileOut ;
    fpos_t  fileInSize , fileOutSize , i ;
    int chr , ret , colCount , colSize = 0 ;
    
    // Initial messages.
    printf( "Bin To Hex - Version 0.1.0\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 3 )
    {
        printf( "Usage: bin2hex <Binary File In> <Hex File Out> [-c50]\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }
    else if( argc > 3 )
    {
        char * str = strstr( argv[ 3 ] , "-c" ) ;
        if( str == argv[ 3 ] )
        {
            colSize = atoi( &argv[ 3 ][ 2 ] ) ;
            printf( "\tConfigured %d columns.\n" , colSize ) ;
        }
    }
    
    // Open Binary File.
    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }

    // Create Hex File.
    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
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
        
        fileOutSize += ( fpos_t ) ret ;
    }
    
    printf( "\t\"%s\" - Size = %lu\n" , argv[ 2 ] , ( unsigned long ) fileOutSize ) ;
    printf( "\tDone!\n" ) ;
    
    return( 0 ) ;
}
