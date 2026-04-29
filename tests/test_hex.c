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
/*
    unsigned char input[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    char output[8];

    size_t written = bin_to_hex(input, sizeof(input), output);

    check(written == 8, "multiple bytes: size mismatch");
    check(memcmp(output, "DEADBEEF", 8) == 0, "multiple bytes: wrong hex value");
*/
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

    print_testCounters( testCounters ) ;

    return 0;
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
