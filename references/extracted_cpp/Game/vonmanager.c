// Source: F:/Games/Dayz/scripts/3_game/vonmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VONManagerBase : Managed
{
    protected bool                  m_VoNToggled;
    ref       ScriptInvoker         m_OnVonStateEvent;
    ref       ScriptInvoker         m_OnPartyChatChangedEvent;
    
    void VONManagerBase()
    {
        m_VoNToggled = false;
