/**
 * @file binHistogram.h
 * @author Francesco Sacco
 * @date 10 Dec 2018
 * @brief This project calculates the histogram of the file.
 *
 * This software is a tool to calculate the histogram of the file
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2018-05-18 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h>

int main( int argc , char * argv[] )
{
    FILE * file  = NULL ;
    fpos_t fileInSize ;
    unsigned long int i ;
    int chr , ret ;

    unsigned long int histogram[ 256 ] ;
    float percentage ;
    
    // Initial messages.
    printf( "Bin Histogram - Version 0.0.0\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc != 2 )
    {
        printf( "Usage: binHistogram <Binary File>\n" ) ;
        printf( "\n" ) ;
        return( 0 ) ;
    }

    // Open Binary File.
    file = fopen( argv[ 1 ] , "rb" ) ;
    if( file == ( ( FILE * ) NULL ) )
    {
        printf( "\tError to open \"%s\"\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }

    // Check binary file size.
    fileInSize = 0 ;
    ret = fseek( file , 0 , SEEK_END ) ;
    ret = ( ret == 0 ) ? fgetpos( file , &fileInSize ) : ( ret ) ;
    ret = ( ret == 0 ) ? fseek( file , 0 , SEEK_SET ) : ( ret ) ;
    if( ret != 0 )
    {
        ret = ferror( file ) ;
        printf( "\tError to get binary file size!\n" ) ;
        printf( "\tferror [ %d ]\n" , ret ) ;
        fclose( file ) ;
        return( ret ) ;
    }
    printf( "\t\"%s\" - Size = %lu\n" , argv[ 1 ] , ( unsigned long ) fileInSize ) ;
    
    memset( histogram , 0 , sizeof( histogram ) ) ;

    for( i = 0 ; i < fileInSize ; i++ )
    {
        chr = getc( file ) ;
        if( chr == EOF )
        {
            ret = ferror( file ) ;
            printf( "\tError to read binary file at %lu!\n" , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( file ) ;
            return( ret ) ;
        }    
        
        histogram[ chr & 0xFF ]++ ;
    }
    

    printf( "\n" ) ;
    printf( "Byte     - count      - percentage\n" ) ;
    for( i = 0 ; i < 256 ; i++ )
    {
        percentage  = ( float ) histogram[ i ] ;
	percentage *= 100.0 ;
	percentage /= ( float ) fileInSize ;

        if( ( i >= 0x20 ) && ( i <= 0x7E ) )
	{
            printf( " %02Xh '%c' - " , ( unsigned char ) i , ( char ) i ) ;
	}
	else
	{
	    printf( " %02Xh     - " , ( unsigned char ) i ) ;
	}
	printf( "%10lu - " , histogram[ i ] ) ;
	printf( "%f\n" , percentage ) ;
    }

    fclose( file ) ;

    return( 0 ) ;
}
