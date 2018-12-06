/**
 * @file bin2gray.h
 * @author Francesco Sacco
 * @date 06 Dec 2018
 * @brief This project converts binary files to Gray Code.
 *
 * This software is a tool to convert binary files to Gray
 * code files.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2018-12-06 - 0.0.0 - Initial version.
  *
  **********/
 
#include <stdio.h>
#include <string.h> // To use strstr().
#include <stdlib.h> // To use atoi().

unsigned char bin2gray( unsigned char bin ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn  = NULL ;
    FILE * fileOut = NULL ;
    fpos_t  fileInSize , i ;
    int chr , ret ;
    
    // Initial messages.
    printf( "Bin To Gray - Version 0.0.0\n" ) ;
    printf( "Francesco Sacco - francesco_sacco@hotmail.com\n" ) ;
    
    // Check arguments.
    if( argc < 2 )
    {
        printf( "Usage: bin2gray <Binary File In> <Gray File Out>\n" ) ;
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

    // Create Gray File.
    fileOut = fopen( argv[ 2 ] , "wb" ) ;
    if( fileIn == ( ( FILE * ) NULL ) )
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
    
    for( i = 0 ; i < fileInSize ; i++ )
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
        
	chr = ( int ) bin2gray( ( unsigned char ) chr ) ;
	ret = putc( chr , fileOut ) ;
        if( ret != chr )
        {
            ret = ferror( fileOut ) ;
            printf( "\tError to write gray file!\n" ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( fileIn ) ;
            fclose( fileOut ) ;
            return( ret ) ;
        }
    }
    
    printf( "\tDone!\n" ) ;
    
    return( 0 ) ;
}

unsigned char bin2gray( unsigned char bin )
{
                                           // xx00   xx01   xx10   xx11
    static const unsigned char grayCode[] = { 0x00 , 0x01 , 0x03 , 0x02 ,   // 00xx
	                                      0x06 , 0x07 , 0x05 , 0x04 ,   // 01xx
					      0x0C , 0x0D , 0x0F , 0x0E ,   // 10xx
					      0x0A , 0x0B , 0x09 , 0x08 } ; // 11xx
    unsigned char ret ;

    ret = grayCode[ ( bin & 0xF0 ) >> 4 ] ;
    ret <<= 4 ;
    ret |= grayCode[ bin & 0x0F ] ;

    return( ret ) ;
}

