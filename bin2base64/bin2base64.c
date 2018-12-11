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
  * 2018-12-10 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void bin2base64( uint8_t * binData , uint8_t binDataSize , uint8_t * base64Data ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    fpos_t  fileInSize , i , j ;
    int chr , ret ;
    
    uint8_t bufferBin[ 4 ] ;
    uint8_t buffer64[ 5 ] ;
    
    // Initial messages.
    printf( "Bin To Base64 - Version 0.0.0\n" ) ;
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

void bin2base64( uint8_t * binData , uint8_t binDataSize , uint8_t * base64Data )
{
    static const uint8_t base64_table[] = { 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' ,
                                            'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' ,
                                            'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' ,
                                            'Y' , 'Z' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' ,
                                            'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' ,
                                            'o' , 'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' ,
                                            'w' , 'x' , 'y' , 'z' , '0' , '1' , '2' , '3' ,
                                            '4' , '5' , '6' , '7' , '8' , '9' , '+' , '/' } ;
    
    uint8_t data ;
    
    if( base64Data == NULL )
    {
        return ;
    }
    
    memset( base64Data , '\0' , 5 ) ;
    
    switch( binDataSize )
    {
    // 0 Bytes to convert.
    case 0 :
        // There's no data.
        base64Data[ 0 ] = '\0' ;
        break ;
    case 1 :
        // xxxx.xxxx 0000
        // \_____/\_____/
        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data &= 0x30 ;
        base64Data[ 1 ] = base64_table[ data ] ;

        base64Data[ 2 ] = '=' ;
        base64Data[ 3 ] = '=' ;
        break ;
    case 2 :
        // xxxx.xxxx xxxx.xxxx 00
        // \_____/\_____/ \_____/

        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data |= binData[ 1 ] >> 4 ;
        data &= 0x3F ;
        base64Data[ 1 ] = base64_table[ data ] ;

        data  = binData[ 1 ] << 2 ;
        data &= 0x3C ;
        base64Data[ 2 ] = base64_table[ data ] ;

        base64Data[ 3 ] = '=' ;
        break ;
    // 3 bytes to convert.
    default :
        // xxxx.xxxx xxxx.xxxx xxxx.xxxx
        // \_____/\_____/ \_____/\_____/

        data = binData[ 0 ] >> 2 ;
        data &= 0x3F ;
        base64Data[ 0 ] = base64_table[ data ] ;

        data  = binData[ 0 ] << 4 ;
        data |= binData[ 1 ] >> 4 ;
        data &= 0x3F ;
        base64Data[ 1 ] = base64_table[ data ] ;

        data  = binData[ 1 ] << 2 ;
        data |= binData[ 2 ] >> 6 ;
        data &= 0x3F ;
        base64Data[ 2 ] = base64_table[ data ] ;

        data  = binData[ 2 ] ;
        data &= 0x3F ;
        base64Data[ 3 ] = base64_table[ data ] ;
        break ;
    }
}
