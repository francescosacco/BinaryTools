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
    print_testResult( result , "Invalid character not detected." ) ;
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
    print_testResult( result , "Odd length not detected." ) ;
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
