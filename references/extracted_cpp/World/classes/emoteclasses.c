// Source: F:/Games/Dayz/scripts/4_world/classes/emoteclasses/emoteclasses.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class EmoteGreeting extends EmoteBase
{
    void EmoteGreeting()
    {
        m_ID = EmoteConstants.ID_EMOTE_GREETING;
        m_InputActionName = &quot;EmoteGreeting&quot;;
        m_StanceMaskAdditive = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_StanceMaskFullbody = DayZPlayerConstants.STANCEMASK_PRONE;
        m_AdditiveCallbackUID = DayZPlayerConstants.CMD_GESTUREMOD_GREETING;
        m_FullbodyCallbackUID = DayZPlayerConstants.CMD_GESTUREFB_GREETING;
        m_HideItemInHands = false;
