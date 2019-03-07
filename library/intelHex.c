/**
 * @file intelHex.h
 * @author Francesco Sacco
 * @date 28 Feb 2019
 * @brief This library convert Intel Hex strings.
 *
 * This library can convert Intel hex strings to binary data.
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2019-02-28 - 0.0.0 - Initial version.
  *
  **********/
#include "intelHex.h"

#include <stdio.h> // To use NULL.
#include <string.h> // To use strlen().
#include "hex2bin.h" // To use isValidHexString(), hex2bin() and bin2hex().

#define IHEX_STARTCODE                           ':'
#define IHEX_STRMINSIZE                          11

uint8_t iHexCalcCks( iHex_t * iHex_stIn ) ;

iHexRet_t iHexToBin( char * strIn , iHex_t * iHex_stOut )
{
    uint16_t i , index ;
    uint8_t hexStrRet , cks ;
    size_t strSize ;
    
    // Check if the parameters are valid pointers.
    if( ( strIn == NULL ) || ( iHex_stOut == NULL ) )
    {
        return( iHexRet_error ) ;
    }

    hexStrRet = isValidHexString( &strIn[ 1 ] ) ;
    strSize = strlen( strIn ) ;
    index = 0 ;

    // Check if the start code is wrong, if the string is not hex, or of the size is less than minimum.
    if( ( strIn[ 0 ] != IHEX_STARTCODE ) || ( !hexStrRet ) || ( strSize < IHEX_STRMINSIZE ) )
    {
        return( iHexRet_error ) ;
    }
    
    iHex_stOut->startCode = strIn[ index++ ] ;
    iHex_stOut->byteCount = hex2bin( strIn[ index += 2 ] ) ;
    
    // Check if the payload size is included in the string size.
    if( ( iHex_stOut->byteCount ) != ( strSize - IHEX_STRMINSIZE ) )
    {
        return( iHexRet_error ) ;
    }

    // Convert address.
    iHex_stOut->addr   = ( uint16_t ) hex2bin( strIn[ index += 2 ] ) ;
    iHex_stOut->addr <<= 8 ;
    iHex_stOut->addr  |= ( uint16_t ) hex2bin( strIn[ index += 2 ] ) ;

    // Convert type.
    iHex_stOut->recType = ( iHexType_t ) hex2bin( strIn[ index += 2 ] ) ;
    
    // Convert payload.
    for( i = 0 ; i < iHex_stOut->byteCount ; i++ )
    {
        iHex_stOut->data[ i ] = hex2bin( strIn[ index += 2 ] ) ;
    }

    // Convert check-sum.
    iHex_stOut->checkSum = hex2bin( strIn[ index += 2 ] ) ;

    // Calculate check-sum from header and payload.
    cks = iHexCalcCks( iHex_stOut ) ;
    // Is it different?
    if( iHex_stOut->checkSum != cks )
    {
        // Yes.
        // So, return error.
        return( iHexRet_error ) ;
    }
    
    return( iHexRet_ok ) ;
}

iHexRet_t binToiHex( iHex_t * iHex_stIn , char * strOut )
{
    uint16_t i , index ;
    uint8_t addrMsb , addrLsb ;
    uint8_t type ;
    uint8_t cks ;
    
    // Check if the parameters are valid pointers.
    if( ( iHex_stIn == NULL ) || ( strOut == NULL ) )
    {
        return( iHexRet_error ) ;
    }
    
    index = 0 ;
    
    addrMsb = ( uint8_t ) ( iHex_stIn->addr >> 8 ) ;
    addrLsb = ( uint8_t ) ( iHex_stIn->addr      ) ;
    type    = ( uint8_t ) ( iHex_stIn->recType   ) ;
    
    strOut[ index++ ] = IHEX_STARTCODE ;
    bin2hex( iHex_stIn->byteCount , &strOut[ index += 2 ] ) ;
    bin2hex( addrMsb              , &strOut[ index += 2 ] ) ;
    bin2hex( addrLsb              , &strOut[ index += 2 ] ) ;
    bin2hex( addrLsb              , &strOut[ index += 2 ] ) ;
    bin2hex( type                 , &strOut[ index += 2 ] ) ;

    for( i = 0 ; i < iHex_stIn->byteCount ; i++ )
    {
        bin2hex( iHex_stIn->data[ i ] , &strOut[ index += 2 ] ) ;
    }
    
    cks = iHexCalcCks( iHex_stIn ) ;
    bin2hex( cks , &strOut[ index += 2 ] ) ;

    strOut[ index ] = '\0' ;
    
    return( iHexRet_ok ) ;
}

uint8_t iHexCalcCks( iHex_t * iHex_stIn )
{
    uint16_t i , iHexSize ;
    uint8_t cks ;

    // Payload size.
    iHexSize = ( uint16_t ) iHex_stIn->byteCount ;

    // Calculate checksum with header.
    cks  = iHex_stIn->byteCount ;
    cks += ( uint8_t ) ( iHex_stIn->addr >> 8 ) ;
    cks += ( uint8_t ) ( iHex_stIn->addr      ) ;
    cks += ( uint8_t ) ( iHex_stIn->recType   ) ;

    // Calculate checksum with data.
    for( i = 0 ; i < iHexSize ; i++ )
    {
        cks += iHex_stIn->data[ i ] ;
    }

    // Two's complement.
    cks = ~cks ;
    cks += 1 ;

    return( cks ) ;
}
