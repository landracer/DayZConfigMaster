// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/triggerevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class TriggerEvents : ScriptedEntity
{   
    protected ref ScriptInvoker     m_OnEnterInvoker;
    protected ref ScriptInvoker     m_OnLeaveInvoker;
    
    ScriptInvoker GetOnEnterInvoker()
    {
        if ( !m_OnEnterInvoker )
            m_OnEnterInvoker = new ScriptInvoker;
        return m_OnEnterInvoker;
