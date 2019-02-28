#include "binaryMemory.h"

#include <stdint.h> // To use NULL.
#include <stdlib.h> // To use free().
#include <string.h> // To use memset() and memcopy().

#define BM_NULL                                  ( ( uint8_t * ) NULL )

static uint8_t * bmMemory = BM_NULL ;
static uint32_t  bmSize   =       0 ;

bmRet_t bmInit( void )
{
    if( bmMemory != BM_NULL )
    {
        free( ( void * ) bmMemory ) ;
        bmMemory = BM_NULL ;
        bmSize = 0 ;
    }
    
    return( bmRet_ok ) ;
}

bmRet_t bmWrite( uint32_t addr , uint8_t * data , uint32_t size )
{
    uint32_t finalSize ;
    uint8_t * finalMemory ;
    
    // Will it have problem to manage this amount of memory?
    if( ( UINT_LEAST32_MAX - addr ) < size )
    {
        // Yes.
        // So, return error.
        return( bmRet_memLimit ) ;
    }
    
    // Calculate final size.
    finalSize = addr + size ;
    // Do we need to allocate more memory?
    if( bmSize < finalSize )
    {
        // Yes.
        finalMemory = ( uint8_t * ) realloc( ( void * ) bmMemory , ( size_t ) finalSize ) ;
        // Did it have problem to allocate?
        if( finalMemory == BM_NULL )
        {
            // Yes.
            // So, return error.
            return( bmRet_outOfMem ) ;
        }
        
        // Fill in the new block of memory.
        ( void ) memset( ( void * ) ( finalMemory + bmSize ) , 0xFF , ( size_t ) size ) ;
        
        // Copy new address and size of memory.
        bmMemory = finalMemory ;
        bmSize   = finalSize   ;
    }

    // Copy new data to the memory.
    ( void ) memcpy( ( void * ) ( bmMemory + addr ) , ( const void * ) data , ( size_t ) size ) ;

    return( bmRet_ok ) ;
}

bmRet_t bmRead( uint32_t addr , uint8_t * data , uint32_t size )
{
    uint32_t finalSize ;

    // Will it have problem to manage this amount of memory?
    if( ( UINT_LEAST32_MAX - addr ) < size )
    {
        // Yes.
        // So, return error.
        return( bmRet_memLimit ) ;
    }
    
    // Calculate final size.
    finalSize = addr + size ;
    // Is the last position to read outside of memory?
    if( finalSize > bmSize )
    {
        // Yes.
        // So, return error.
        return( bmRet_memLimit ) ;
    }

    // Copy data.
    ( void ) memcpy( ( void * ) data , ( const void * ) ( bmMemory + addr ) , ( size_t ) size ) ;

    return( bmRet_ok ) ;
}

bmRet_t bmDeInit( void )
{
    if( bmMemory != BM_NULL )
    {
        free( ( void * ) bmMemory ) ;
        bmMemory = BM_NULL ;
        bmSize = 0 ;
    }
    
    return( bmRet_ok ) ;
}

uint32_t bmGetSize( void )
{
    uint32_t ret ;

    ret = ( bmMemory != BM_NULL ) ? ( bmSize ) : ( 0 ) ;

    return( ret ) ;
}
