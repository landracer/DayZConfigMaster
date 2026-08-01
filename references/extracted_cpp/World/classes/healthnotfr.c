// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/healthnotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HealthNotfr: NotifierBase
{
    /*
    private const float     HEALTHY_BLOOD_TRESHOLD  = 5000;
    private const float     HEALTHY_TRESHOLD        = 5000;
    private const float     HEALING_ENERGY_TRESHOLD = 4000;
    private const float     HEALING_WATER_TRESHOLD  = 2500;
    private const float     HEALING_BLOOD_TRESHOLD  = 5000;
    */
    private const float     DEC_TRESHOLD_LOW            = 0;
    private const float     DEC_TRESHOLD_MED            = -0.7;
    private const float     DEC_TRESHOLD_HIGH           = -1.3;
    private const float     INC_TRESHOLD_LOW            = 0;
    private const float     INC_TRESHOLD_MED            = 0.7;
    private const float     INC_TRESHOLD_HIGH           = 1.3;
    
        
    void HealthNotfr(NotifiersManager manager)
    {
        m_TickInterval = 3000;
        m_TendencyBufferSize = 6;
