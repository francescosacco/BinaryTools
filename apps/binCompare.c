/**
 * @file binCompare.h
 * @author Francesco Sacco
 * @date 25 May 2018
 * @brief This project compare two binaries files.
 *
 * This software is a tool to compare two binaries files. It'll
 * show if there are any difference.
 * @see http://github.com/francescosacco/BinaryTools
 */
 
 /**********
  *
  * Version log. 
  *
  * 2026-04-30 - 1.0.0 - Small changes for new architecture environment.
  * 2019-05-24 - 0.0.3 - Add first byte different.
  * 2019-02-26 - 0.0.2 - Add getFileSize function and difference size.
  * 2018-05-25 - 0.0.1 - Fix data type to print at console.
  * 2018-05-22 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

uint32_t getFileSize( FILE * in ) ;

int main( int argc , char * argv[] )
{
    FILE * file1  ;
    FILE * file2 ;
    uint32_t fileSize1 , fileSize2 , smallestSize , differences , i , addrFirstDif ;
    int chr1 , chr2 ;
    
    // Initial messages.
    printf( "bin Compare - Version 1.0.0\n" ) ;
    printf( "github.com/francescosacco/BinaryTools\n" ) ;    

    // Check arguments.
    if( argc < 3 )
    {
        printf( "Usage: %s <Binary File 1> <Binary File 2>\n" , argv[ 0 ] ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }
    
    // Open Binary File 1.
    file1 = fopen( argv[ 1 ] , "rb" ) ;
    if( file1 == ( ( FILE * ) NULL ) )
    {
        printf( "\tError openning %s.\n" , argv[ 1 ] ) ;
        perror( "\tError" ) ;
        return errno ;
    }
    
    // Open Binary File 2.
    file2 = fopen( argv[ 2 ] , "rb" ) ;
    if( file2 == ( ( FILE * ) NULL ) )
    {
        int errno_fopen = errno ;

        printf( "\tError openning %s.\n" , argv[ 2 ] ) ;
        perror( "\tError" ) ;

        fclose( file1 ) ;
        return errno_fopen ;
    }
    
    // Check binary files size.
    fileSize1 = getFileSize( file1 ) ;
    fileSize2 = getFileSize( file2 ) ;

    // Check if file size is equal.
    if( fileSize1 != fileSize2 )
    {
        if( fileSize1 > fileSize2 )
        {
            printf( "\tFile \"%s\" is %lu bigger than \"%s\"\n" , argv[ 1 ] , ( unsigned long ) ( fileSize1 - fileSize2 ) , argv[ 2 ] ) ;
            smallestSize = fileSize2 ;
        }
        else
        {
            printf( "\tFile \"%s\" is %lu bigger than \"%s\"\n" , argv[ 2 ] , ( unsigned long ) ( fileSize2 - fileSize1 ) , argv[ 1 ] ) ;
            smallestSize = fileSize1 ;
        }
    }
    else
    {
        smallestSize = fileSize1 ;
    }
    
    for( i = 0 , differences = 0 , addrFirstDif = 0 ; i < smallestSize ; i++ )
    {
        chr1 = getc( file1 ) ;
        if( chr1 == EOF )
        {
            int errno_fopen = errno ;

            printf( "\tError to read file \"%s\"at %lu!\n" , argv[ 1 ] , ( unsigned long ) i ) ;
            perror( "\tError" ) ;

            fclose( file1 ) ;
            fclose( file2 ) ;

            return( errno_fopen ) ;
        }    
        
        chr2 = getc( file2 ) ;
        if( chr2 == EOF )
        {
            int errno_fopen = errno ;

            printf( "\tError to read file \"%s\"at %lu!\n" , argv[ 1 ] , ( unsigned long ) i ) ;
            perror( "\tError" ) ;

            fclose( file1 ) ;
            fclose( file2 ) ;

            return( errno_fopen ) ;
        }    

        if( chr1 != chr2 )
        {
            if( differences == 0 )
            {
                addrFirstDif = i ;
            }
            
            differences++ ;
        }
    }
    
    if( differences )
    {
        printf( "\tFound %lu different bytes.\n" , ( unsigned long ) differences ) ;
        printf( "\tThe first difference appears at %lu Byte.\n" , ( unsigned long ) addrFirstDif ) ;
    }
    else if( fileSize1 != fileSize2 )
    {
        printf( "\tThe size is different, but ther's no data difference.\n" ) ;
    }
    else
    {
        printf( "\tFound no differences.\n" ) ;
    }

    printf( "\tDone!\n" ) ;

    fclose( file1 ) ;
    fclose( file2 ) ;
    
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
