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
  * 2019-02-26 - 0.0.2 - Add getFileSize function and difference size.
  * 2018-05-25 - 0.0.1 - Fix data type to print at console.
  * 2018-05-22 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <stdint.h>

uint32_t getFileSize( FILE * in ) ;

int main( int argc , char * argv[] )
{
    FILE * file1  ;
    FILE * file2 ;
    fpos_t  fileSize1 , fileSize2 , smallestSize , differences , i ;
    int chr1 , chr2 , ret ;
    
    // Initial messages.
    printf( "Bin Compare - Version 0.0.1\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 3 )
    {
        printf( "Usage: binCompare <Binary File 1> <Binary File 2>\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }
    
    // Open Binary File 1.
    file1 = fopen( argv[ 1 ] , "rb" ) ;
    if( file1 == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    
    // Open Binary File 2.
    file2 = fopen( argv[ 2 ] , "rb" ) ;
    if( file2 == ( ( FILE * ) NULL ) )
    {
        fclose( file1 ) ;
        printf( "\tError to open \"%s\"\n" , argv[ 2 ] ) ;
        return( -1 ) ;
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
    
    for( i = 0 , differences = 0 ; i < smallestSize ; i++ )
    {
        chr1 = getc( file1 ) ;
        if( chr1 == EOF )
        {
            ret = ferror( file1 ) ;
            printf( "\tError to read file \"%s\"at %lu!\n" , argv[ 1 ] , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( file1 ) ;
            fclose( file2 ) ;
            return( ret ) ;
        }    
        
        chr2 = getc( file2 ) ;
        if( chr2 == EOF )
        {
            ret = ferror( file2 ) ;
            printf( "\tError to read file \"%s\"at %lu!\n" , argv[ 2 ] , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( file1 ) ;
            fclose( file2 ) ;
            return( ret ) ;
        }    

        if( chr1 != chr2 )
        {
            differences++ ;
        }
    }
    
    if( differences )
    {
        printf( "\tFound differences at %lu bytes.\n" , ( unsigned long ) differences ) ;
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
