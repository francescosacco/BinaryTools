/**
 * @file utils.c
 * @author Francesco Sacco
 * @date 10 May 2026
 * @brief This library manage the conversion strings.
 *
 * This library was developed to convert strings from different
 * data types.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

#include "utils.h"

int parse_number( const char * str , long * value )
{
    char * endptr ;
    errno = 0 ;

    long result = strtol( str , &endptr , 0 ) ;

    // No number to convert.
    if( endptr == str )
    {
        return 0 ;
    }

    // Invalid character.
    if( *endptr != '\0' )
    {
        return 0 ;
    }

    // Check if Overflow.
    if( ( errno == ERANGE ) || ( result < 0 ) || ( result == LONG_MAX ) )
    {
        return 0 ;
    }

    *value = result ;

    return 1 ;
}
