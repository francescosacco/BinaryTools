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
  * 2019-03-01 - 0.1.2 - Fixed new library.
  * 2019-02-12 - 0.1.1 - Change exhibition of file name.
  * 2019-02-11 - 0.1.0 - Created library with hex2bin code.
  * 2019-02-09 - 0.0.1 - Fix upper case.
  * 2019-02-08 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../library/hexString.h"

#define CHR_CR                                   '\r'
#define CHR_LF                                   '\n'

uint32_t getFileSize( FILE * in ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    char * fileInName = NULL ;
    char * fileOutName = NULL ;

    int fileInNameSize ;
    int fileOutNameSize ;
    int fileNameSize ;
    char fileName[ 64 ] ;

    uint32_t fileInSize , fileOutSize , i ;
    char hexData[ 2 ] ;
    uint8_t validRet , binData ;
    int chr , ret , hexDataIdx ;
    
    // Initial messages.
    printf( "Hex To Bin - Version 0.1.2\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc != 3 )
    {
        printf( "Usage: hex2bin <Hex File In> <Bin File Out>\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }

    fileInName  = argv[ 1 ] ;
    fileOutName = argv[ 2 ] ;
    
    fileInNameSize  = ( int ) strlen( ( const char * ) fileInName  ) ;
    fileOutNameSize = ( int ) strlen( ( const char * ) fileOutName ) ;

    // Open Hex File.
    fileIn = fopen( fileInName , "rb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , fileInName ) ;
        return( -1 ) ;
    }

    // Create Hex File.
    fileOut = fopen( fileOutName , "wb" ) ;
    if( fileOut == ( ( FILE * ) NULL ) )
    {
        fclose( fileIn ) ;
        printf( "\tError to create \"%s\"\n" , fileOutName ) ;
        return( -1 ) ;
    }
    
    // Check hex file size.
    fileInSize = getFileSize( fileIn ) ;
    
    for( i = 0 , fileOutSize = 0 , hexDataIdx = 0 ; i < fileInSize ; i++ )
    {
        chr = getc( fileIn ) ;
        if( chr == EOF )
        {
            ret = ferror( fileIn ) ;
            printf( "\tError to read hex file at %lu!\n" , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return( ret ) ;
        }
        
        if( ( chr == CHR_CR ) || ( chr == CHR_LF ) )
        {
            continue ;
        }
        
        hexData[ hexDataIdx++ ] = ( uint8_t ) chr ;

        if( hexDataIdx >= 2 )
        {
            validRet = isValidHex( hexData ) ;
            if( validRet == 0 )
            {
                printf( "\tError hex character at %lu!\n" , ( unsigned long ) i ) ;
                fclose( fileIn ) ;
                fclose( fileOut ) ;
                return( -1 ) ;
            }

            binData = hex2bin( hexData ) ;
            ( void ) fwrite( ( const void * ) &binData , 1 , 1 , fileOut ) ;
            hexDataIdx = 0 ;
            fileOutSize++ ;
        }
    }

    fileNameSize = ( fileInNameSize > fileOutNameSize ) ? ( fileInNameSize ) : ( fileOutNameSize ) ;
    if( fileNameSize > 63 )
    {
        fileNameSize = 63 ;
    }

    ( void ) strncpy( fileName , ( const char * ) fileInName , fileNameSize ) ;
    if( fileNameSize > fileInNameSize )
    {
        ( void ) memset( fileName + fileInNameSize , ' ' , fileNameSize - fileInNameSize ) ;
    }
    fileName[ fileNameSize ] = '\0' ;

    printf( "\t%s - Size = %lu Bytes\n" , fileName , ( unsigned long ) fileInSize ) ;

    ( void ) strncpy( fileName , ( const char * ) fileOutName , fileNameSize ) ;
    if( fileNameSize > fileOutNameSize )
    {
        ( void ) memset( fileName + fileOutNameSize , ' ' , fileNameSize - fileOutNameSize ) ;
    }
    fileName[ fileNameSize ] = '\0' ;
    
    printf( "\t%s - Size = %lu Bytes\n" , fileName , ( unsigned long ) fileOutSize ) ;
	printf( "\tDone!\n" ) ;
    
    fclose( fileIn ) ;
    fclose( fileOut ) ;

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
