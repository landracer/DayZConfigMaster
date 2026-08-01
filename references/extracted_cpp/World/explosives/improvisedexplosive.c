// Source: F:/Games/Dayz/scripts/4_world/entities/explosivesbase/improvisedexplosive.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ImprovisedExplosive : ExplosivesBase
{
    protected const float TIME_TRIGGER_INITIAL_DELAY_SECS           = 0.1;
    protected const float TIME_TRIGGER_TIMER_BASED_DELAY_SECS       = 1.0;
    protected const float TIME_TRIGGER_DELAY_SECS                   = 0.3;
    
    protected static const string SLOT_TRIGGER_ALARM_CLOCK          = &quot;TriggerAlarmClock&quot;;
    protected static const string SLOT_TRIGGER_KITCHEN_TIMER        = &quot;TriggerKitchenTimer&quot;;
    protected static const string SLOT_TRIGGER_REMOTE               = &quot;TriggerRemoteDetonator_Receiver&quot;;
    
    protected static const string SLOT_EXPLOSIVE_A                  = &quot;IEDExplosiveA&quot;;
    protected static const string SLOT_EXPLOSIVE_B                  = &quot;IEDExplosiveB&quot;;
    
    protected const int SLOT_EXPLOSIVE_COUNT                        = 2;
    protected const string SLOT_EXPLOSIVES[SLOT_EXPLOSIVE_COUNT]    = {
        SLOT_EXPLOSIVE_A,
        SLOT_EXPLOSIVE_B
