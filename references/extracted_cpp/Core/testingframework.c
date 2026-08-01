// Source: F:/Games/Dayz/scripts/2_gamelib/tests/testingframework.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
//-----------------------------------------------------------------------------
class Test
{
    string Suite;
 
    int TimeoutS;
    int TimeoutMs;
    int SortOrder;

    void Test(string suite, int timeoutS = 0, int timeoutMs = 0, int sortOrder = 0)
    {
        Suite = suite;
        TimeoutS = timeoutS;
        TimeoutMs = timeoutMs;
        SortOrder = sortOrder;
