#include "cindivid.h"

#include <string>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------

namespace CGen {
//------------------------------------------------------------

CIndivid::CIndivid( long long size )
    : m_state(0)
{
    resize( size );
    memset( m_state, 0, size );
}

//------------------------------------------------------------

CIndivid::~CIndivid()
{
    if ( m_state )
        delete[] m_state;
}

//------------------------------------------------------------

void CIndivid::resize( long long newSize )
{
   if ( newSize <= 0 )
       throw std::string( "CIndivid trying to resize with incorrect parameter. " ).append( __FUNCTION__ );

   if ( m_size == newSize )
       return;

    if ( m_state )
        delete[] m_state;

    m_size = newSize;
    m_state = new bool[ newSize ];
}

//------------------------------------------------------------

BASETYPE CIndivid::calculateFitness( CFitnessClass* fClass )
{
    if ( !fClass )
        throw std::string( "CIndivid is trying to calculate fitness with (NULL) func" ).append( __FUNCTION__ );  

    m_fitness = (*fClass)( *this );

    return m_fitness;
}

//------------------------------------------------------------

void CIndivid::repair( CRepairClass* repClass )
{
    if ( !repClass )
        throw std::string( "CIndivid is trying to repair with (NULL) func" ).append( __FUNCTION__ );

    (*repClass)( m_state, m_size );
}

//------------------------------------------------------------

void CIndivid::generate( const CProbVec& probVec )
{
    if ( probVec.size() != m_size )
        throw std::string( "CIndivid is trying to generate with incorrect size vec. " ).append( __FUNCTION__ );

    for ( long long i = 0; i < m_size; ++i )
    {
        const BASETYPE randVal = BASETYPE( rand() ) / RAND_MAX;
        m_state[i] = randVal <= probVec.data()[i];
    }
}

//------------------------------------------------------------

CIndivid& CIndivid::operator=( const CIndivid& rInd )
{
    resize( rInd.m_size );
    memcpy( m_state, rInd.m_state, m_size );
    m_fitness = rInd.m_fitness;
    return *this;
}

//------------------------------------------------------------
}
