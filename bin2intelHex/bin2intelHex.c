/**
 * @file bin2intelHex.h
 * @author Francesco Sacco
 * @date 25 Feb 2019
 * @brief This project converts binary files to Intel HEX files.
 *
 * This software is a tool to convert binary files to Intel HEX.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-27 - 0.0.2 - Fix file manipulation.
  * 2019-02-26 - 0.0.1 - Add enum to type and remove size limite.
  * 2019-02-25 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>

typedef enum
{
    iHex_data            = 0x00 ,
    iHex_endOfFile       = 0x01 ,
    iHex_ExtendedSegAddr = 0x02 ,
    iHex_StartSegAddr    = 0x03 ,
    iHex_ExtLinearAddr   = 0x04 ,
    iHex_StartLinearAddr = 0x05
} iHexType_t ;

uint32_t getFileSize( FILE * in ) ;
uint32_t intelHexLine( uint8_t size , uint16_t addr , iHexType_t type , uint8_t * data , char * outPut ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    char * fileInName = NULL ;
    char * fileOutName = NULL ;
    uint32_t fileInSize , fileOutSize , i ;
    int chr , ret ;

    uint8_t buffer[ 16 ] ;
    char bufferOut[ 64 ] ;
    uint8_t bufferIndex ;
    uint32_t addrCount ;
    
    // Initial messages.
    printf( "Bin To Intel Hex - Version 0.0.2\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc != 3 )
    {
        printf( "Usage: bin2intelHex <Binary File In> <Hex File Out>\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }

    fileInName  = argv[ 1 ] ;
    fileOutName = argv[ 2 ] ;

    // Open Binary File.
    fileIn = fopen( fileInName , "rb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , fileInName ) ;
        return( -1 ) ;
    }

    // Check binary file size.
    fileInSize = getFileSize( fileIn ) ;
    printf( "\t\"%s\" - Size = %lu\n" , fileInName , ( unsigned long ) fileInSize ) ;
    
    // Create Intel Hex File.
    fileOut = fopen( fileOutName , "wb" ) ;
    if( fileOut == ( ( FILE * ) NULL ) )
    {
        fclose( fileIn ) ;
        printf( "\tError to create \"%s\"\n" , fileOutName ) ;
        return( -1 ) ;
    }
    
    for( i = 0 , fileOutSize = 0 , bufferIndex = 0 , addrCount = 0 ; i < fileInSize ; i++ )
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
        
        buffer[ bufferIndex++ ] = ( uint8_t ) chr ;
        
        if( bufferIndex >= 16 )
        {
            ( void ) intelHexLine( bufferIndex , ( uint16_t ) addrCount , iHex_data , buffer , bufferOut ) ;
            fileOutSize += ( uint32_t ) fprintf( fileOut , "%s\n" , bufferOut ) ;
            addrCount += bufferIndex ;
            bufferIndex = 0 ;
            
            // Already turn 64KBytes.
            if( ( addrCount & 0x0000FFFF ) == 0x00000000 )
            {
                buffer[ 0 ] = ( uint8_t ) ( addrCount >> 24 ) ;
                buffer[ 1 ] = ( uint8_t ) ( addrCount >> 16 ) ;
                ( void ) intelHexLine( 2 , 0x0000 , iHex_ExtendedSegAddr , buffer , bufferOut ) ;
                fileOutSize += ( uint32_t ) fprintf( fileOut , "%s\n" , bufferOut ) ;
            }
        }
    }

    if( bufferIndex )
    {
        ( void ) intelHexLine( bufferIndex , addrCount , iHex_data , buffer , bufferOut ) ;
        fileOutSize += ( uint32_t ) fprintf( fileOut , "%s\n" , bufferOut ) ;
    }

    ( void ) intelHexLine( 0x00 , 0x0000 , iHex_endOfFile , NULL , bufferOut ) ;
    fileOutSize += ( uint32_t ) fprintf( fileOut , "%s\n" , bufferOut ) ;
    
    printf( "\t\"%s\" - Size = %lu\n" , fileOutName , ( unsigned long ) fileOutSize ) ;
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

uint32_t intelHexLine( uint8_t size , uint16_t addr , iHexType_t type , uint8_t * data , char * outPut )
{
    int outIndex = 0 ;
    uint8_t cks ;
            
    // Start Code + Byte Count.
    outIndex = sprintf( outPut , ":%02X" , size ) ;
    cks = size ;
            
    // Address.
    outIndex += sprintf( outPut + outIndex , "%04X" , addr ) ;
    cks += ( uint8_t ) ( addr >> 8 ) ;
    cks += ( uint8_t ) ( addr      ) ;
    
    // Record type.
    outIndex += sprintf( outPut + outIndex , "%02X" , ( uint8_t ) type ) ;
    cks += ( uint8_t ) type ;
            
    // Data.
    for( int i = 0 ; ( i < size ) && ( data != NULL ) ; i++ )
    {
        outIndex += sprintf( outPut + outIndex , "%02X" , data[ i ] ) ;
        cks += data[ i ] ;
    }
    
    // Two's complement.
    cks  = ~cks ;
    cks += 1 ;

    // Check sum.
    outIndex += sprintf( outPut + outIndex , "%02X" , cks ) ;

    return( ( uint32_t ) outIndex ) ;
}
