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
  * 2019-05-25 - 0.0.2 - Improve ASCII table.
  * 2019-02-13 - 0.0.1 - Add documentation.
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

    /**********
     *
     * Histogram data: The index of the data represents the byte. The content of the
     *                 memory is the number of times this bytes appears in the file.
     *
     *        +-----+
     *   FFh  |     | -> Number of times of byte FFh.
     *        +-----+
     *   FEh  |     | -> Number of times of byte FEh.
     *        +-----+
     *        |     |
     *
     *        |     |
     *        +-----+
     *        |     | -> Number of times of byte 01h.
     *   01h  +-----+
     *   00h  |     | -> Number of times of byte 00h.
     *        +-----+
     *
     **********/
    
    unsigned long int histogram[ 256 ] ;
    float percentage ;
    
    // Initial messages.
    printf( "Bin Histogram - Version 0.0.2\n" ) ;
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
    printf( "\t\"%s\" - Size = %lu Bytes.\n" , argv[ 1 ] , ( unsigned long ) fileInSize ) ;
    
    // Initialize histogram with 0.
    memset( histogram , 0 , sizeof( histogram ) ) ;

    // Runs calculation byte by byte.
    for( i = 0 ; i < fileInSize ; i++ )
    {
        // Read data from file.
        chr = getc( file ) ;
        if( chr == EOF )
        {
            ret = ferror( file ) ;
            printf( "\tError to read binary file at %lu!\n" , ( unsigned long ) i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( file ) ;
            return( ret ) ;
        }    
        
        // Increments the position that represents the character.
        histogram[ chr & 0x00FF ]++ ;
    }

    // Prints the results.
    printf( "\n" ) ;
    printf( "Byte     -      count - percentage\n" ) ;
    for( i = 0 ; i < 256 ; i++ )
    {
        static const char * asciiCtrl[] = { "NUL" , "SOH" , "STX" , "ETX" , "EOT" , "ENQ" , "ACK" , "BEL" ,
                                            "BS " , "HT " , "LF " , "VT " , "FF " , "CR " , "SO " , "SI " ,
                                            "DLE" , "DC1" , "DC2" , "DC3" , "DC4" , "NAK" , "SYN" , "ETB" ,
                                            "CAN" , "EM " , "SUB" , "ESC" , "FS " , "GS " , "RS " , "US " } ;

        // Calculate percentage of the data.
        percentage  = ( float ) histogram[ i ] ;
        percentage *= 100.0 ;
        percentage /= ( float ) fileInSize ;

        // Print ASCII table.
        if( i < 0x20 )
        {
            // Print control character.
            printf( " %02Xh %s - " , ( unsigned char ) i , asciiCtrl[ i ] ) ;
        }
        else if( ( i >= 0x20 ) && ( i <= 0x7E ) )
        {
            // Print ASCII character.
            printf( " %02Xh '%c' - " , ( unsigned char ) i , ( char ) i ) ;
        }
        else
        {
            // Keeps space.
            printf( " %02Xh     - " , ( unsigned char ) i ) ;
        }

        printf( "%10lu - " , histogram[ i ] ) ;
        printf( "%f\n" , percentage ) ;
    }

    fclose( file ) ;

    return( 0 ) ;
}
