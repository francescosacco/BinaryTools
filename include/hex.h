/**
 * @file hex.h
 * @author Francesco Sacco
 * @date 29 Apr 2026
 * @brief This library manage the conversion between HEX and BIN.
 *
 * This library was developed to manage the conversion between binary data
 * and hexadecimal data.
 *
 * @see http://github.com/francescosacco/BinaryTools
 */

 /**********
  *
  * Version log. 
  *
  * 2026-04-29 - 0.0.0 - Initial version.
  * 2026-05-11 - 0.1.0 - Add Intel Hex converter.
  *
  **********/

#ifndef HEX_H
#define HEX_H

#include <stdint.h>
#include <stddef.h>

// Error status.
typedef enum
{
    HEX_OK = 0 ,
    HEX_ERR_INVALID_CHAR ,
    HEX_ERR_ODD_LENGTH ,
    HEX_ERR_INVALID_ARG ,
} hex_status_t ;

/**
 * @bried Convert a buffer of data from binary to hex.
 *
 * @param input      Input buffer with binady data.
 * @param input_len  Sizeo fo the input buffer, in bytes.
 * @param output     Output buffer, with the converted hex data.
 */
size_t bin_to_hex( const uint8_t * input , size_t input_len , char * output ) ;

/**
 * @brief Convert hexadecimal text into binary.
 *
 * @param input       Buffer with hex ASCII content.
 * @param input_len   Buffer size.
 * @param output      Output buffer.
 * @param output_len  Output buffer length.
 */
hex_status_t hex_to_bin( const char * input , size_t input_len , unsigned char * output , size_t * output_len) ;

/**
 * @brief Convert binary data into Intel Hex.
 *
 * @param output      Output buffer.
 * @param len         Data length to be converted.
 * @param addr        Address of the initial data.
 * @param type        Intel Hex command type.
 * @param data        Binary data buffer to be converted.
 *
 * @return Output buffeer size.
 */
size_t ihex_write_record( char * output , unsigned char len , unsigned short addr , unsigned char type , const unsigned char * data ) ;

/**
 * @brief Convert a pair of hex data into binary.
 *
 * @param s  String with at least a pair of hex data.
 *
 * @return Value in binary, between 0 and 255. It'll be -1 if the string is not valid.
 */
int hex_byte( const char * s ) ;

/**
 * @brief Calculate and compare the checksum of a IHX line.
 *
 * @param line  String with just one line of Intel Hex string.
 *
 * @return Return 1 if the checksum us valid, 0 if it's not valid.
 */
int validate_checksum( const char * line , int len ) ;

#endif // HEX_H
