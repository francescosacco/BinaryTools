/**
 * @file bin2base32.c
 * @author Francesco Sacco
 * @date 12 Dec 2018
 * @brief This project converts binary files to Base-32 files.
 *
 * This software is a tool to convert binary files to Base-32
 * files.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2018-12-12 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void bin2base32( uint8_t * binData , uint8_t binDataSize , uint8_t * base32Data ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    fpos_t  fileInSize , i , j ;
    int chr , ret ;
    
    uint8_t bufferBin[ 5 ] ;
    uint8_t buffer32[ 9 ] ;
    
    // Initial messages.
    printf( "Bin To Base32 - Version 0.0.0\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 3 )
    {
        printf( "Usage: bin2base32 <Binary File In> <Base32 File Out>\n" ) ;
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

    // Create Base32 File.
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
        for( j = 0 ; ( j < 5 ) && ( i < fileInSize ) ; j++, i++ )
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
        
        bin2base32( bufferBin , j , buffer32 ) ;
        
        ret = fprintf( fileOut , "%s" , buffer32 ) ;
        if( ret < 0 )
        {
            ret = ferror( fileOut ) ;
            printf( "\tError to write base32 file!\n" ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return( ret ) ;
        }
    }
    
    printf( "\tDone!\n" ) ;
    
    return( 0 ) ;
}

void bin2base32( uint8_t * binData , uint8_t binDataSize , uint8_t * base32Data )
{
    static const uint8_t base32_table[] = { 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' ,
                                            'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' ,
                                            'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' ,
                                            'Y' , 'Z' , '2' , '3' , '4' , '5' , '6' , '7' } ;
    
    uint8_t binDataCopy[ 5 ] ;
    uint8_t data , binDataCopySize ;
    
    if( ( base32Data == NULL ) || ( binDataSize == 0 ) )
    {
        return ;
    }

    memset( base32Data  , 0x00 ,                     9 ) ;
    memset( binDataCopy , 0x00 , sizeof( binDataCopy ) ) ;
    binDataCopySize = ( binDataSize > 5 ) ? ( 5 ) : ( binDataSize ) ;
    memcpy( binDataCopy , binData , binDataCopySize ) ;

    // xxxx.xxxx xxxx.xxxx xxxx.xxxx xxxx.xxxx xxxx.xxxx
    // \____/\____/\____/\____/ \____/\____/\____/\____/

    data  = binDataCopy[ 0 ] >> 3 ; // xxx76543
    data &= 0x1F                  ; // ...76543
    base32Data[ 0 ] = base32_table[ data ] ;
    
    data  = binDataCopy[ 0 ] << 2 ; // 543210xx
    data |= binDataCopy[ 1 ] >> 6 ; // xxxxxx76
    data &= 0x1F                  ; // ...21076
    base32Data[ 1 ] = base32_table[ data ] ;

    data  = binDataCopy[ 1 ] >> 1 ; // x7654321
    data &= 0x1F                  ; // ...54321
    base32Data[ 2 ] = base32_table[ data ] ;

    data  = binDataCopy[ 1 ] >> 1 ; // x7654321
    data &= 0x1F                  ; // ...54321
    base32Data[ 2 ] = base32_table[ data ] ;

    data  = binDataCopy[ 1 ] << 4 ; // 3210xxxx
    data |= binDataCopy[ 2 ] >> 4 ; // xxxx7654
    data &= 0x1F                  ; // ...07654
    base32Data[ 3 ] = base32_table[ data ] ;

    data  = binDataCopy[ 2 ] << 1 ; // 6543210x
    data |= binDataCopy[ 3 ] >> 7 ; // xxxxxxx7
    data &= 0x1F                  ; // ...32107
    base32Data[ 4 ] = base32_table[ data ] ;

    data  = binDataCopy[ 3 ] >> 2 ; // xx765432
    data &= 0x1F                  ; // ...65432
    base32Data[ 5 ] = base32_table[ data ] ;

    data  = binDataCopy[ 3 ] << 3 ; // 43210xxx
    data |= binDataCopy[ 4 ] >> 5 ; // xxxxx765
    data &= 0x1F                  ; // ...10765
    base32Data[ 6 ] = base32_table[ data ] ;

    data  = binDataCopy[ 4 ]      ; // 76543210
    data &= 0x1F                  ; // ...43210
    base32Data[ 7 ] = base32_table[ data ] ;
    
    switch( binDataCopySize )
    {
    case 1 : // 1Byte -> 6 paddings.
        memset( base32Data + 2 , '=' , 6 ) ;
        break ;
    case 2 : // 2Byte -> 4 paddings.
        memset( base32Data + 4 , '=' , 4 ) ;
        break ;
    case 3 : // 3Byte -> 3 paddings.
        memset( base32Data + 5 , '=' , 3 ) ;
        break ;
    case 4 : // 4Byte -> 1 paddings.
        memset( base32Data + 7 , '=' , 1 ) ;
        break ;
    }
}
