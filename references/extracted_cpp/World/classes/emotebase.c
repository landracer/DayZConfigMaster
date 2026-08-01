// Source: F:/Games/Dayz/scripts/4_world/classes/emoteclasses/emotebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EmoteBase
{
    protected PlayerBase m_Player; //useful for various conditions in child classes
    protected int m_ID = -1;
    protected string m_InputActionName = &quot;&quot;;
    protected int m_StanceMaskAdditive = 0;
    protected int m_StanceMaskFullbody = 0;
    protected int m_AdditiveCallbackUID = 0;
    protected int m_FullbodyCallbackUID = 0;
    protected bool m_HideItemInHands = false;
    
    bool EmoteCondition(int stancemask)
    {
        return true;
