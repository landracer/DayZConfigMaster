// Source: F:/Games/Dayz/scripts/3_game/ai/aigroup.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AIGroup : Managed
{
    private void AIGroup();
    private void ~AIGroup();
    
    proto native void AddAgent(notnull AIAgent agent);
    proto native void RemoveAgent(notnull AIAgent agent);
    
    proto native AIGroupBehaviour GetBehaviour();
