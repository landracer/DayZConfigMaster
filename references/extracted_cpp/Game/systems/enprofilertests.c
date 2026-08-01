// Source: F:/Games/Dayz/scripts/3_game/systems/tftests/enprofilertests.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EnProfilerTests : TestFramework
{
    bool m_bWasProfilerEnabled;
    
    //---------------------------------------------------------------------------
    // Ctor - Decides the tests to run
    //---------------------------------------------------------------------------
    void EnProfilerTests()
    {
        m_bWasProfilerEnabled = EnProfiler.IsEnabledC();
        
        AddInitTest(&quot;TestToggling&quot;);
        AddInitTest(&quot;TestTogglingImmediate&quot;);
        AddInitTest(&quot;TestSetFlags&quot;);
        AddInitTest(&quot;TestClearFlags&quot;);
        AddInitTest(&quot;TestAddFlags&quot;);
        AddInitTest(&quot;TestModule&quot;);
        AddInitTest(&quot;TestClassTimeData&quot;);
        AddInitTest(&quot;TestClassCountData&quot;);
        AddInitTest(&quot;TestFuncTimeData&quot;);
        AddInitTest(&quot;TestFuncCountData&quot;);       
