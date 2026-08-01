// Source: F:/Games/Dayz/scripts/3_game/autotest/autotestrunner.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    This class is just a convenience wrapper for using the actual TestHarness.
*/
class AutotestRunner
{
    private static bool m_IsRunning;
    private static bool m_IsDone;
    
    static bool IsRunning()
    {
        return m_IsRunning;
