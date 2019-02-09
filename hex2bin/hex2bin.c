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
  * 2019-02-09 - 0.0.1 - Fix upper case.
  * 2019-02-08 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h> // To use strstr().
#include <stdlib.h> // To use atoi().
#include <stdint.h>

#define CHR_CR                                   '\r'
#define CHR_LF                                   '\n'


uint32_t getFileSize( FILE * in ) ;

uint8_t isValidHex( uint8_t * hexIn ) ;
uint8_t hex2bin( uint8_t * hexIn ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    char * fileInName = NULL ;
    char * fileOutName = NULL ;
    uint32_t fileInSize , fileOutSize , i ;
    uint8_t hexData[ 2 ] ;
    uint8_t validRet , binData ;
    int chr , ret , hexDataIdx ;
    
    // Initial messages.
    printf( "Hex To Bin - Version 0.0.1\n" ) ;
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
    printf( "\t\"%s\" - Size = %lu\n" , fileInName , ( unsigned long ) fileInSize ) ;
    
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
    
    printf( "\t\"%s\" - Size = %lu\n" , fileOutName , ( unsigned long ) fileOutSize ) ;
	printf( "\tDone!\n" ) ;
    
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

uint8_t isValidHex( uint8_t * hexIn )
{
    char * validHex = "0123456789abcdefABCDEF" ;
    char * chrRet ;
    
    chrRet = strchr( validHex , ( int ) hexIn[ 0 ] ) ;
    if( chrRet == NULL )
    {
        return( 0 ) ;
    }

    chrRet = strchr( validHex , ( int ) hexIn[ 1 ] ) ;
    if( chrRet == NULL )
    {
        return( 0 ) ;
    }

    return( 1 ) ;
}

uint8_t hex2bin( uint8_t * hexIn )
{
    uint8_t chrHigh , chrLow , ret ;

    chrHigh = hexIn[ 0 ] & 0xDF ;
    chrLow  = hexIn[ 1 ] & 0xDF ;
    
    chrHigh = ( chrHigh & 0x40 ) ? ( chrHigh - 'A' + 0x0A ) : ( chrHigh - '0' ) ;
    chrLow  = ( chrLow  & 0x40 ) ? ( chrLow  - 'A' + 0x0A ) : ( chrLow  - '0' ) ;
    
    ret   = chrHigh ;
    ret <<= 4 ;
    ret  |= chrLow ;
    
    return( ret ) ;
}
