#ifndef CPROB_VEC_H
#define CPROB_VEC_H

#include "defs.h"

//------------------------------------------------------------

namespace CGen {
//------------------------------------------------------------

class CIndivid;

//------------------------------------------------------------

class CProbVec
{
public:
    CProbVec( long long size );
    ~CProbVec(); 

    void update( const CIndivid& best, const CIndivid& lose );

    void resize( long long size );
    void setInitial();

    inline long long size() const { return m_size; }

    const BASETYPE* data() const { return m_state; }

    CProbVec& operator=( const CProbVec& rVec );

private:
    BASETYPE* m_state;
    long long m_size;
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
