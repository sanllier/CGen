#include "cprobvec.h"
#include "cindivid.h"

#include <string>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------

namespace CGen {
//------------------------------------------------------------

CProbVec::CProbVec( long long size )
    : m_state(0)
{
    resize( size );
    setInitial();
}

//------------------------------------------------------------

CProbVec::~CProbVec()
{
    if ( m_state )
        delete[] m_state;
}

//------------------------------------------------------------

void CProbVec::update( const CIndivid& best, const CIndivid& lose )
{
    if ( best.size() != lose.size() )
        throw std::string( "ÑCProbVec trying to update with different dims individs. " ).append( __FUNCTION__ );

    resize( best.size() );

    const BASETYPE frac = BASETYPE(1) / BASETYPE( m_size );
    for ( long long i = 0; i < m_size; ++i )
        if ( best.data()[i] != lose.data()[i] )
            m_state[i] += best.data()[i] ? +frac : -frac;
}

//------------------------------------------------------------

void CProbVec::resize( long long size )
{
    if ( size <= 0 )
       throw std::string( "ÑCProbVec trying to resize with incorrect parameter. " ).append( __FUNCTION__ );

    if ( m_size == size )
        return;

    if ( m_state )
        delete[] m_state;
    m_state = new BASETYPE[ size_t(size) ];
    m_size = size;
}

//------------------------------------------------------------

void CProbVec::setInitial()
{
    for ( long long i = 0; i < m_size; ++i )
        m_state[i] = BASETYPE( rand() ) / RAND_MAX;//BASETYPE(0.5);
}

//------------------------------------------------------------

CProbVec& CProbVec::operator=( const CProbVec& rVec )
{
    resize( rVec.m_size );
    memcpy( m_state, rVec.m_state, size_t( m_size * sizeof( BASETYPE ) ) );
    return *this;
}

//------------------------------------------------------------
}
