#ifndef CGEN_H
#define CGEN_H

#include "defs.h"
#include "cprobvec.h"
#include "cindivid.h"

#include <string>

//------------------------------------------------------------

namespace CGen {
//------------------------------------------------------------

class CProcessScreen
{
public:
    virtual void operator()( long long cycle, const CIndivid& individ ) = 0;
};

//------------------------------------------------------------

struct SCGenParams
{
    long long cycThreshold;
    int individsNum;
    int indSize;
    BASETYPE targetFitness;
    BASETYPE accuracy;
    CFitnessClass*  fClass;
    CRepairClass*   repClass;
    CProcessScreen* screenClass;
    std::string outFile;

    SCGenParams( const char* file = 0, CFitnessClass* fC = 0, CRepairClass* rC = 0, CProcessScreen* sC = 0 );

    void init( const char* file, CFitnessClass* fC, CRepairClass* rC = 0, CProcessScreen* sC = 0 );
};

//------------------------------------------------------------

class CGenProcess
{
public:
    CGenProcess( const SCGenParams& params );
    ~CGenProcess();

    double process();

    const CIndivid& getBestIndivid() const;

private:
    SCGenParams m_params;

    CIndivid* m_best;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
