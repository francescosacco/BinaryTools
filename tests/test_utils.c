#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

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

static void test_decimal(void)
{
    long value = 0 ;
    char * valuestr = "123" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( value == 123 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Decimal value match." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_hexadecimal(void)
{
    long value = 0 ;
    char * valuestr = "0xF03C" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( value == 0xF03C ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Hexadecimal value match." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_invalidString(void)
{
    long value = 0 ;
    char * valuestr = "abc" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_partialNumber(void)
{
    long value = 0 ;
    char * valuestr = "123abc" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_emptyString(void)
{
    long value = 0 ;
    char * valuestr = "" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_halfHex(void)
{
    long value = 0 ;
    char * valuestr = "0x123" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 1 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;

    result = ( value == 0x123 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Hexadecimal value match." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

static void test_decimalOverflow(void)
{
    long value = 0 ;
    char * valuestr = "999999999999999999999999999" ;

    testCounters_incrementGroup( &testCounters ) ;

    int resultParser = parse_number( valuestr , &value ) ;

    TestResult_t result = ( resultParser == 0 ) ? ( testResult_success ) : ( testResult_fail ) ;
    print_testResult( result , "Conversion result." ) ;
    testCounters_incrementResult( &testCounters , result ) ;
}

// Executor.

int main(void)
{
    printf("UTILS Library tests:\n");

    testCounters_reset( &testCounters );

    printf( "Test: test_decimal\n" ) ;
    test_decimal() ;

    printf( "Test: test_hexadecimal\n" ) ;
    test_hexadecimal() ;

    printf( "Test: test_invalidString\n" ) ;
    test_invalidString() ;

    printf( "Test: test_partialNumber\n" ) ;
    test_partialNumber() ;

    printf( "Test: test_emptyString\n" ) ;
    test_emptyString() ;

    printf( "Test: test_halfHex\n" ) ;
    test_halfHex() ;
    
    printf( "Test: test_decimalOverflow\n" ) ;
    test_decimalOverflow() ;

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
