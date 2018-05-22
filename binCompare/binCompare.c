/**
 * @file binCompare.h
 * @author Francesco Sacco
 * @date 22 May 2018
 * @brief This project compare two binaries files.
 *
 * This software is a tool to compare two binaries files. It'll
 * show if there are any difference.
 * @see http://github.com/francescosacco/BinaryTools
 */
#include <stdio.h>

int main( int argc , char * argv[] )
{
    FILE * file1  ;
    FILE * file2 ;
    fpos_t  fileSize1 , fileSize2 , smallestSize , differences , i ;
    int chr1 , chr2 , ret ;
    
    // Initial messages.
    printf( "Bin Compare - Version 0 - 2018-05-22\n" ) ;
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
    
    // Check binary file 1 size.
    fileSize1 = 0 ;
    ret = fseek( file1 , 0 , SEEK_END ) ;
    ret = ( ret == 0 ) ? fgetpos( file1 , &fileSize1 ) : ( ret ) ;
    ret = ( ret == 0 ) ? fseek( file1 , 0 , SEEK_SET ) : ( ret ) ;
    if( ret != 0 )
    {
        ret = ferror( file1 ) ;
        printf( "\tError to get binary file \"%s\" size!\n" , argv[ 1 ] ) ;
        printf( "\tferror [ %d ]\n" , ret ) ;
        fclose( file1 ) ;
        fclose( file2 ) ;
        return( ret ) ;
    }
    
    // Check binary file 2 size.
    fileSize2 = 0 ;
    ret = fseek( file2 , 0 , SEEK_END ) ;
    ret = ( ret == 0 ) ? fgetpos( file2 , &fileSize2 ) : ( ret ) ;
    ret = ( ret == 0 ) ? fseek( file2 , 0 , SEEK_SET ) : ( ret ) ;
    if( ret != 0 )
    {
        ret = ferror( file2 ) ;
        printf( "\tError to get binary file \"%s\" size!\n" , argv[ 2 ] ) ;
        printf( "\tferror [ %d ]\n" , ret ) ;
        fclose( file1 ) ;
        fclose( file2 ) ;
        return( ret ) ;
    }

    // Check if file size os equal.
    if( fileSize1 != fileSize2 )
    {
        if( fileSize1 > fileSize2 )
        {
            printf( "\tFile \"%s\" is %u bigger than \"%s\"\n" , argv[ 1 ] , ( unsigned long ) ( fileSize1 - fileSize2 ) , argv[ 2 ] ) ;
            smallestSize = fileSize2 ;
        }
        else
        {
            printf( "\tFile \"%s\" is %u bigger than \"%s\"\n" , argv[ 2 ] , ( unsigned long ) ( fileSize2 - fileSize1 ) , argv[ 1 ] ) ;
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
            printf( "\tError to read file \"%s\"at %u!\n" , argv[ 1 ] , i ) ;
            printf( "\tferror [ %d ]\n" , ret ) ;
            fclose( file1 ) ;
            fclose( file2 ) ;
            return( ret ) ;
        }    
        
        chr2 = getc( file2 ) ;
        if( chr2 == EOF )
        {
            ret = ferror( file2 ) ;
            printf( "\tError to read file \"%s\"at %u!\n" , argv[ 2 ] , i ) ;
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
        printf( "\tFound differences at %u bytes.\n" , differences ) ;
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
