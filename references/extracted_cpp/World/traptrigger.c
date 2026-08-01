// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/traptrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class TrapTrigger : Trigger
{
    bool m_Enabled = false;
    TrapBase m_ParentObj;
    
    #ifdef DEVELOPER
    bool m_CanSendDbg = true;
    #endif
    
    void TrapTrigger()
    {
        int mask = GetEventMask();
        mask &= ~EntityEvent.INIT;
        SetEventMask(mask);
