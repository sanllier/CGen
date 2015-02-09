#include "cgen.h"
#include "sharedmtrand.h"
#include "pugixml.hpp"

#include <string>
#include <vector>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

//------------------------------------------------------------

namespace CGen {
//-----------------------------------------------------------

SCGenParams::SCGenParams( const char* file/* = 0*/, CFitnessClass* fC/* = 0*/, CRepairClass* rC/* = 0*/, CProcessScreen* sC/* = 0*/)
        : cycThreshold(0)
        , individsNum(0)
        , indSize(0)
        , targetFitness( BASETYPE(0) )
        , accuracy( BASETYPE(0) )
        , fClass(0) 
        , repClass(0)
        , screenClass(0)
{
    if ( file )
        init( file, fC, rC, sC );
}

//-----------------------------------------------------------

void SCGenParams::init( const char* file, CFitnessClass* fC, CRepairClass* rC/* = 0*/, CProcessScreen* sC/* = 0*/)
{
    if ( !file || !file[0] )
        throw std::string( "Some problems with params file. " ).append( __FUNCTION__ );

    pugi::xml_document doc;
    doc.load_file( file );
    
    pugi::xml_node qgenNode = doc.child( "qgen" );    
    if ( !qgenNode )
        throw std::string( "Some problems with params file. " ).append( __FUNCTION__ );

    for ( pugi::xml_node node = qgenNode.child( "parameter" ); node; node = node.next_sibling() )
    {
        const char* name = node.attribute( "name" ).as_string();
        if ( !name )
            continue;

        if ( 0 == strcmp( "cycle-threshold", name ) )
        {
            cycThreshold = (long long)node.attribute( "value" ).as_uint(0);
        }
        else if ( 0 == strcmp( "individs-num", name ) )
        {
            individsNum = (int)node.attribute( "value" ).as_int(0);
        }
        else if ( 0 == strcmp( "individ-size", name ) )
        {
            indSize = (int)node.attribute( "value" ).as_int(0);
        }
        else if ( 0 == strcmp( "target-fitness", name ) )
        {
            targetFitness = (BASETYPE)node.attribute( "value" ).as_double(0.0);
        }
        else if ( 0 == strcmp( "target-accuracy", name ) )
        {
            accuracy = (BASETYPE)node.attribute( "value" ).as_double(0.0);
        }
        else if ( 0 == strcmp( "out-file", name ) )
        {
            outFile = node.attribute( "value" ).as_string("");
        }
    }

    fClass      = fC;
    repClass    = rC;
    screenClass = sC;
}

//-----------------------------------------------------------

CGenProcess::CGenProcess( const SCGenParams& params )
    : m_params( params ) 
    , m_best(0)
{
    if ( !m_params.fClass )
        throw std::string( "Invalid fitness class: NULL pointer." ).append( __FUNCTION__ ); 

    SharedMTRand::getClosedInstance();

    m_best = new CIndivid( m_params.indSize );

    srand( ( unsigned int )( time(0) ) );
}

//-----------------------------------------------------------

CGenProcess::~CGenProcess()
{
    if ( m_best )
        delete m_best;
}

//-----------------------------------------------------------

const CIndivid& CGenProcess::getBestIndivid() const
{
    return *m_best;
}

//-----------------------------------------------------------

double CGenProcess::process()
{
    if ( m_params.individsNum < 1 )
        throw std::string( "CGenProcess trying to process with too small number of individs(must be >=1). " ).append( __FUNCTION__ ); 

    double startTime = double( time(0) );
    std::vector< CProbVec* > probVecPull( m_params.indSize );
    std::vector< CIndivid* > indsPull( m_params.indSize );
    for ( int i = 0; i < m_params.individsNum; ++i )
    {
        probVecPull[i] = new CProbVec( m_params.indSize );
        indsPull[i]    = new CIndivid( m_params.indSize );
    }

    CIndivid* best = new CIndivid( m_params.indSize );
    CIndivid* lose = new CIndivid( m_params.indSize );

    for ( long long cycle = 1; cycle <= m_params.cycThreshold; ++cycle )
    {
        #pragma omp parallel for
        for ( int i = 0; i < m_params.individsNum; ++i )
        {
            best->generate( *probVecPull[i] );
            lose->generate( *probVecPull[i] );

            best->calculateFitness( m_params.fClass );
            lose->calculateFitness( m_params.fClass );

            if ( lose->getFitness() > best->getFitness() )
            {
                CIndivid* temp = best;
                best = lose;
                lose = temp;
            }

            if ( ( cycle == 1 && i == 0 ) || best->getFitness() > m_best->getFitness() )
                *m_best = *best;

            *indsPull[i] = *best;
        }

        #pragma omp parallel for
        for ( int i = 0; i < m_params.individsNum; ++i )
            probVecPull[i]->update( *m_best, *indsPull[i] );

        if ( m_params.screenClass )
            ( *m_params.screenClass )( cycle, *m_best );

        if ( m_params.targetFitness > 0.0f )
        {
            if ( m_params.accuracy > 0.0f )
            {
                if ( fabs( m_best->getFitness() - m_params.targetFitness ) <= m_params.accuracy )
                    break;
            }
            else if ( m_best->getFitness() >= m_params.targetFitness )
                break;
        }
    }

    for ( int i = 0; i < m_params.individsNum; ++i )
    {
        delete probVecPull[i];
        delete indsPull[i];
    }

    delete best;
    delete lose;

    return time(0) - startTime;
}

//------------------------------------------------------------
}
