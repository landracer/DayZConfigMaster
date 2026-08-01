// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/animatedactionbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBaseCB : HumanCommandActionCallback
{
    protected ActionData                m_ActionData;
    protected SoundOnVehicle            m_SoundObject;//object of sound playing on entity
    protected bool                      m_Canceled;//helps prevent doubled calling of actionbase End method
    protected bool                      m_Interrupted;//force callback to wait till action syncs its interruption
        
        
    void ActionBaseCB()
    {
