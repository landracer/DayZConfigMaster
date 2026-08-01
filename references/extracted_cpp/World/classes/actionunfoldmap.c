// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionunfoldmap.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnfoldMapCB : ActionBaseCB
{
    bool                                m_MapFolding;
    bool                                m_HasReceivedEvent;
    bool                                m_CancelCondition;
    int                                 m_InitMovementState;
    int                                 m_FinalMovementState;
    
    void ActionUnfoldMapCB()
    {
        RegisterAnimationEvent(&quot;ActionExec&quot;, UA_ANIM_EVENT);
        EnableStateChangeCallback();
        EnableCancelCondition(true);
