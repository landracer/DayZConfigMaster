// Source: F:/Games/Dayz/scripts/3_game/ai/aiagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AIAgent : Managed
{
    private void AIAgent();
    private void ~AIAgent();
    
    proto native void SetKeepInIdle(bool enable);
    proto native AIGroup GetGroup();
