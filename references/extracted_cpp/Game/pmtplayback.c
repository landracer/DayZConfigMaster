// Source: F:/Games/Dayz/scripts/3_game/particles/tests/pmtplayback.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PMTPlayback : PMTF
{
    ref array<ParticleSource> m_ParticleSources = new array<ParticleSource>();
    
    // TestOnePlaying
    int m_OnePlayingManagerID;
    bool m_bOnePlayingTestSuccess = false;
    
    // TestOnePlayingStandAloneAutoDestroy
    int m_OnePlayingSAADPSID;
    bool m_bOnePlayingSAADEnded = false;
    
    // TestOnePlayingStandAlone
    int m_OnePlayingSAPSID;
    bool m_bOnePlayingSAEnded = false;
    
    // TestStop
    int m_StopPSID;
    float m_StopAccumulatedTime;
    static const float STOP_ACCUMULATED_TIME_STOP_CUTOFF = 2;
    static const float STOP_ACCUMULATED_TIME_PLAY_CUTOFF = 3;
    bool m_bStopWasStopped = false;
    bool m_bStopWasResumed = false;
    bool m_bStopEnded = false;
    
    //---------------------------------------------------------------------------
    // Ctor - Decides the tests to run
    //---------------------------------------------------------------------------
    void PMTPlayback()
    {
        //AddInitTest(&quot;TestOnePlaying&quot;);
        //AddInitTest(&quot;TestOnePlayingStandAloneAutoDestroy&quot;);
        //AddInitTest(&quot;TestOnePlayingStandAlone&quot;);
        //AddInitTest(&quot;TestWiggleStress&quot;);
        AddInitTest(&quot;TestStopping&quot;);
