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


// Error status,
typedef enum
{
    HEX_OK = 0,
    HEX_ERR_INVALID_CHAR,
    HEX_ERR_ODD_LENGTH
} hex_status_t;

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

typedef enum
{
    IHEX_OK = 0 ,
    IHEX_ERR_INVALID_ARG ,
} ihex_status_t ;

typedef enum
{
    IHEX_MODE_LINEAR,
    IHEX_MODE_SEGMENT
} ihex_mode_t;

size_t ihex_write_record( char * output , unsigned char len , unsigned short addr , unsigned char type , const unsigned char * data ) ;
ihex_status_t bin_to_ihex_buffer( const unsigned char * data , size_t data_len , unsigned int base_addr , ihex_mode_t mode , char * line_buf , size_t line_buf_size ) ;

#endif // HEX_H
