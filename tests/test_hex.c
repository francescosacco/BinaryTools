#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hex.h"

typedef enum
{
    testResult_success ,
    testResult_fail
} TestResult_t ;

typedef struct
{
    uint32_t testCount ;
    uint32_t testGroupsCount ;
    uint32_t testSuccessful ;
    uint32_t testFailed ;
} TestCounters_t ;

static TestCounters_t testCounters ;

// Support functions:
void print_testResult( TestResult_t testResult , const char * msg ) ;
void print_testCounters( TestCounters_t testCounters ) ;

void testCounters_reset( TestCounters_t * testCounters ) ;
void testCounters_incrementGroup( TestCounters_t * testCounters ) ;
void testCounters_incrementResult( TestCounters_t * testCounters , TestResult_t testResult ) ;

/* ---------- Testes ---------- */

static void test_single_byte(void)
{
    unsigned char input[] = { 0x00 };
    char output[2];

    testCounters_incrementGroup( &testCounters ) ;

    size_t written = bin_to_hex(input, 1, output);

    TestResult_t result = ( written == 2 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Two bytes written." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( memcmp(output, "00", 2) == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Match data." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_multiple_bytes(void)
{
    unsigned char input[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    char output[8];

    testCounters_incrementGroup( &testCounters ) ;

    size_t written = bin_to_hex(input, sizeof(input), output);

    TestResult_t result = ( written == 8 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Eight bytes written." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( memcmp(output, "DEADBEEF", 8) == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Match data." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_empty_buffer(void)
{
    unsigned char input[] = {};
    char output[1];

    testCounters_incrementGroup( &testCounters ) ;

    output[0] = 0x5A ;
    size_t written = bin_to_hex(input, 0, output);

    TestResult_t result = ( written == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Zero bytes written." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( output[0] == 0x5A ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Match data." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_invalid_char(void)
{
    const char *input = "DEADZEEF";
    unsigned char output[4];
    size_t out_len = 0 ;

    testCounters_incrementGroup( &testCounters ) ;

    hex_status_t status = hex_to_bin( input , strlen( input ) , output , &out_len ) ;

    TestResult_t result = ( status == HEX_ERR_INVALID_CHAR ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Invalid character." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_odd_length(void)
{
    const char * input = "ABC" ;
    unsigned char output[ 2 ] ;
    size_t out_len = 0 ;

    testCounters_incrementGroup( &testCounters ) ;

    hex_status_t status = hex_to_bin( input , strlen( input ) , output , &out_len ) ;

    TestResult_t result = ( status == HEX_ERR_ODD_LENGTH ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Odd length." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_hex_byte_basic(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    result = ( hex_byte( "00" ) == 0x00 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"00\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "AB" ) == 0xAB ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"AB\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "aB" ) == 0xAB ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"aB\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "ab" ) == 0xAB ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"ab\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "0A" ) == 0x0A ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"0A\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "0a" ) == 0x0a ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"0a\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "F3" ) == 0xF3 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"F3\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "f3" ) == 0xF3 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"f3\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "FF" ) == 0xFF ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"FF\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "12" ) == 0x12 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"12\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_hex_byte_invalid(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    result = ( hex_byte( "G0" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"G0\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "0G" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"0G\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "ZZ" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"ZZ\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "#@" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"#@\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_hex_byte_mixed(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    result = ( hex_byte( "A#" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"A#\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "#A" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"#A\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "0*" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"0*\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( hex_byte( "&0" ) == -1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test the value \"&0\"." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_check_sum_simple_line(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    const char *line1 = "0400000001020304F2" ;
    const char *line2 = "00000001FF" ;
    const char *line3 = "020000040000FA" ;

    result = ( validate_checksum( line1 , strlen( line1 ) ) ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Validate checksum simple line." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( validate_checksum( line2 , strlen( line2 ) ) ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Validate checksum EOF." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( validate_checksum( line3 , strlen( line3 ) ) ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Validate checksum Extended Linear." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_check_sum_simple_invalid_cks(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    const char * line1 = "0400000001020304F3";

    result = ( validate_checksum( line1 , strlen( line1 ) ) == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test invalid checksum." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_check_sum_simple_invalid_size(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    const char * line1 = "0400000001020304F" ;

    result = ( validate_checksum( line1 , strlen( line1 ) ) == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test invalid size." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_check_sum_simple_invalid_data(void)
{
    testCounters_incrementGroup( &testCounters ) ;

    TestResult_t result ;

    const char * line1 = "04000000010203ZZF2";

    result = ( validate_checksum( line1 , strlen( line1 ) ) == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Test invalid data." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

// Executor.

int main(void)
{
    printf("HEX Library tests:\n");

    testCounters_reset( &testCounters );

    printf( "Test: test_single_byte\n" ) ;
    test_single_byte() ;

    printf( "Test: test_multiple_bytes\n" ) ;
    test_multiple_bytes() ;

    printf( "Test: test_empty_buffer\n" ) ;
    test_empty_buffer() ;

    printf( "Test: test_invalid_char\n" ) ;
    test_invalid_char() ;

    printf( "Test: test_odd_length\n" ) ;
    test_odd_length() ;
    
    printf( "Test: test_hex_byte_basic\n" ) ;
    test_hex_byte_basic() ;

    printf( "Test: test_hex_byte_invalid\n" ) ;
    test_hex_byte_invalid() ;
    
    printf( "Test: test_hex_byte_mixed\n" ) ;
    test_hex_byte_mixed() ;
    
    printf( "Test: test_check_sum_simple_line\n" ) ;
    test_check_sum_simple_line() ;
    
    printf( "Test: test_check_sum_simple_invalid_cks\n" ) ;
    test_check_sum_simple_invalid_cks() ;
    
    printf( "Test: test_check_sum_simple_invalid_data\n" ) ;
    test_check_sum_simple_invalid_data() ;

    print_testCounters( testCounters ) ;

    return 0 ;
}

// Support functions: Implementation.

void print_testResult( TestResult_t testResult , const char * msg )
{
    if( testResult_success == testResult )
    {
        printf( "[  OK  ]" ) ;
    }
    else
    {
        printf( "[ FAIL ]" ) ;
    }
    
    printf( " - \"%s\"\n" , msg ) ;
}

void print_testCounters( TestCounters_t testCounters )
{
    printf( "Test counters:\n" ) ;
    printf( "\tTests groups     : %u\n" , testCounters.testGroupsCount ) ;
    printf( "\tTests Successful : %u\n" , testCounters.testSuccessful ) ;
    printf( "\tTests Failed     : %u\n" , testCounters.testFailed ) ;
    printf( "\tTotal Tests      : %u\n" , testCounters.testCount ) ;
}

void testCounters_reset( TestCounters_t * testCounters )
{
    testCounters->testGroupsCount = 0 ;
    testCounters->testSuccessful = 0 ;
    testCounters->testFailed = 0 ;
    testCounters->testCount = 0 ;
}

void testCounters_incrementGroup( TestCounters_t * testCounters )
{
    testCounters->testGroupsCount++ ;
}

void testCounters_incrementResult( TestCounters_t * testCounters , TestResult_t testResult )
{
    testCounters->testCount++ ;
    
    if( testResult_success == testResult )
    {
        testCounters->testSuccessful++ ;
    }
    else
    {
        testCounters->testFailed++ ;
    }
}
