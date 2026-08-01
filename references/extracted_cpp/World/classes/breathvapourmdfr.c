// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/breathvapourmdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int BREATH_VAPOUR_LEVEL_MAX = 3;
 
class BreathVapourMdfr extends ModifierBase
{
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_ID                    = eModifiers.MDF_BREATH_VAPOUR;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 6;
        DisableDeactivateCheck();
 
