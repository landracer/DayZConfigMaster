// Source: F:/Games/Dayz/scripts/3_game/systems/tftests/scriptinvokertests.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptInvokerTests : TestFramework
{
    ref ScriptInvoker m_Invoker;
    int m_InvokeCount;
    
    //---------------------------------------------------------------------------
    // Ctor - Decides the tests to run
    //---------------------------------------------------------------------------
    void ScriptInvokerTests()
    {
        m_Invoker = new ScriptInvoker();
        
        //AddInitTest(&quot;TestFirstUnique&quot;);
        //AddInitTest(&quot;TestSecondUnique&quot;);
        //AddInitTest(&quot;TestInsertRemoveUnique&quot;);
        //AddInitTest(&quot;TestInsertUniqueImmediate&quot;);
        //AddInitTest(&quot;TestClearRunning&quot;);
        //AddInitTest(&quot;TestInvokeRunning&quot;);
        AddInitTest(&quot;TestInsertRunning&quot;);
