// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/compass.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Compass : ItemCompass
{
    protected ref OpenableBehaviour m_Openable;
    
    void Compass()
    {
        m_Openable = new OpenableBehaviour(false);
        
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
        
        UpdateVisualState();
