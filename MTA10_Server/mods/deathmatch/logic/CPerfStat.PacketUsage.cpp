/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/CPerfStat.PacketUsage.cpp
*  PURPOSE:     Performance stats manager class
*  DEVELOPERS:  Mr OCD
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl
//
//
//
///////////////////////////////////////////////////////////////
class CPerfStatPacketUsageImpl : public CPerfStatPacketUsage
{
public:
    ZERO_ON_NEW

                                CPerfStatPacketUsageImpl  ( void );
    virtual                     ~CPerfStatPacketUsageImpl ( void );

    // CPerfStatModule
    virtual const SString&      GetCategoryName         ( void );
    virtual void                DoPulse                 ( void );
    virtual void                GetStats                ( CPerfStatResult* pOutResult, const std::map < SString, int >& optionMap, const SString& strFilter );

    // CPerfStatPacketUsageImpl
    void                        RecordStats             ( void );

    long long                   m_llNextRecordTime;
    SString                     m_strCategoryName;
    SPacketStat                 m_PacketStats [ 2 ] [ 256 ];
};


///////////////////////////////////////////////////////////////
//
// Temporary home for global object
//
//
//
///////////////////////////////////////////////////////////////
static CPerfStatPacketUsageImpl* g_pPerfStatPacketUsageImp = NULL;

CPerfStatPacketUsage* CPerfStatPacketUsage::GetSingleton ()
{
    if ( !g_pPerfStatPacketUsageImp )
        g_pPerfStatPacketUsageImp = new CPerfStatPacketUsageImpl ();
    return g_pPerfStatPacketUsageImp;
}


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::CPerfStatPacketUsageImpl
//
//
//
///////////////////////////////////////////////////////////////
CPerfStatPacketUsageImpl::CPerfStatPacketUsageImpl ( void )
{
    m_strCategoryName = "Packet usage";
}


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::CPerfStatPacketUsageImpl
//
//
//
///////////////////////////////////////////////////////////////
CPerfStatPacketUsageImpl::~CPerfStatPacketUsageImpl ( void )
{
}

///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::GetCategoryName
//
//
//
///////////////////////////////////////////////////////////////
const SString& CPerfStatPacketUsageImpl::GetCategoryName ( void )
{
    return m_strCategoryName;
}


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::DoPulse
//
//
//
///////////////////////////////////////////////////////////////
void CPerfStatPacketUsageImpl::DoPulse ( void )
{
    // Copy and clear once every 5 seconds
    long long llTime = GetTickCount64_ ();

    if ( llTime >= m_llNextRecordTime )
    {
        m_llNextRecordTime = Max ( m_llNextRecordTime + 5000, llTime + 5000 / 10 * 9 );
        RecordStats ();
    }
}


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::RecordStats
//
//
//
///////////////////////////////////////////////////////////////
void CPerfStatPacketUsageImpl::RecordStats ( void )
{
    memcpy ( m_PacketStats, g_pNetServer->GetPacketStats (), sizeof ( m_PacketStats ) );
    g_pNetServer->ClearPacketStats ();
}


///////////////////////////////////////////////////////////////
//
// CPerfStatPacketUsageImpl::GetStats
//
//
//
///////////////////////////////////////////////////////////////
void CPerfStatPacketUsageImpl::GetStats ( CPerfStatResult* pResult, const std::map < SString, int >& strOptionMap, const SString& strFilter )
{
    //
    // Set option flags
    //
    bool bHelp = MapContains ( strOptionMap, "h" );

    //
    // Process help
    //
    if ( bHelp )
    {
        pResult->AddColumn ( "Packet usage help" );
        pResult->AddRow ()[0] ="Option h - This help";
        return;
    }

    // Add columns
    pResult->AddColumn ( "Packet type" );
    pResult->AddColumn ( "Incoming" );
    pResult->AddColumn ( "pkt/sec" );
    pResult->AddColumn ( "bytes/sec" );
    pResult->AddColumn ( "cpu" );
    pResult->AddColumn ( "Outgoing" );
    pResult->AddColumn ( "pkt/sec" );
    pResult->AddColumn ( "bytes/sec" );
    pResult->AddColumn ( "cpu" );

    // Fill rows
    for ( uint i = 0 ; i < 256 ; i++ )
    {
        const SPacketStat& statIn = m_PacketStats [ CNetServer::STATS_INCOMING_TRAFFIC ] [ i ];
        const SPacketStat& statOut = m_PacketStats [ CNetServer::STATS_OUTGOING_TRAFFIC ] [ i ];

        if ( !statIn.iCount && !statOut.iCount )
            continue;

        // Add row
        SString* row = pResult->AddRow ();

        int c = 0;
        row[c++] = SString ( "%d", i );
        row[c++] = "|";
        if ( statIn.iCount )
        {
            row[c++] = SString ( "%d", statIn.iCount / 5 );
            row[c++] = SString ( "%d", statIn.iTotalBytes / 5 );
            row[c++] = SString ( "%2.2f%%", statIn.totalTime / 50000.f );   // Number of microseconds in sample period ( 5sec * 1000000 ) into percent ( * 100 )
        }
        else
        {
            row[c++] = "-";
            row[c++] = "-";
            row[c++] = "-";
        }

        row[c++] = "|";
        if ( statOut.iCount )
        {
            row[c++] = SString ( "%d", statOut.iCount / 5 );
            row[c++] = SString ( "%d", statOut.iTotalBytes / 5 );
            //row[c++] = SString ( "%2.2f%%", statIn.totalTime / 50000.f );
            row[c++] = "n/a";
        }
        else
        {
            row[c++] = "-";
            row[c++] = "-";
            row[c++] = "-";
        }
    }
}
