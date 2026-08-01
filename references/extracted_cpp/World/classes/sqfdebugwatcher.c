// Source: F:/Games/Dayz/scripts/4_world/classes/sqfdebugwatcher.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SqfDebugWatcher
{
    private int m_Id;
    private string m_SqfCommand;
    
    private bool m_IsRunning;
        
 
    void SqfDebugWatcher( int id, string sqf_command )
    {
        m_Id = id;
        m_SqfCommand = sqf_command;
        m_IsRunning = false;
