#ifndef CINDIVID_H
#define CINDIVID_H

#include "cprobvec.h"
#include "defs.h"

//------------------------------------------------------------

namespace CGen {
//------------------------------------------------------------

class CFitnessClass
{
public:
    virtual ~CFitnessClass() {}
    virtual BASETYPE operator()( const CIndivid& individ ) = 0;    
};

//------------------------------------------------------------

class CRepairClass
{
public:
    virtual ~CRepairClass() {}
    virtual BASETYPE operator()( bool* data, long long size ) = 0;    
};

//------------------------------------------------------------

class CIndivid
{
public:
    CIndivid( long long size );
    ~CIndivid();

    void resize( long long newSize );

    long long size() const { return m_size; } 
    const bool* data() const { return m_state; }

    BASETYPE calculateFitness( CFitnessClass* fClass );
    BASETYPE getFitness() const { return m_fitness; } // unsafe, m_fitness may be dirty

    void repair( CRepairClass* repClass );

    void generate( const CProbVec& probVec );

    CIndivid& operator=( const CIndivid& rInd );

private:
    bool* m_state;
    long long m_size;

    BASETYPE m_fitness;
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
