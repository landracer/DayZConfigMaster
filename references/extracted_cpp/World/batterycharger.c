// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/batterycharger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BatteryCharger extends ItemBase
{
    // Model selections
    static protected const string SEL_CLIPS_CAR         = &quot;clips_car_battery&quot;;
    static protected const string SEL_CLIPS_TRUCK       = &quot;clips_truck_battery&quot;;
    static protected const string SEL_CLIPS_DETACHED    = &quot;clips_detached&quot;;
    static protected const string SEL_CLIPS_FOLDED      = &quot;clips_folded&quot;;
    static protected const string SEL_SWITCH_ON         = &quot;switch_on&quot;;
    static protected const string SEL_SWITCH_OFF        = &quot;switch_off&quot;;
    static protected const string SEL_CORD_PLUGGED      = &quot;cord_plugged&quot;;
    static protected const string SEL_CORD_FOLDED       = &quot;cord_folded&quot;;
    static protected const string SEL_LIGHT_STATE_1     = &quot;light_stand_by&quot;;
    static protected const string SEL_LIGHT_STATE_2     = &quot;light_charging&quot;;
    static protected const string SEL_LIGHT_STATE_3     = &quot;light_charged&quot;;
    
    // glow materials
    static protected const string RED_LIGHT_GLOW        = &quot;dz\\gear\\camping\\data\\battery_charger_light_r.rvmat&quot;;
    static protected const string GREEN_LIGHT_GLOW      = &quot;dz\\gear\\camping\\data\\battery_charger_light_g.rvmat&quot;;
    static protected const string YELLOW_LIGHT_GLOW     = &quot;dz\\gear\\camping\\data\\battery_charger_light_y.rvmat&quot;;
    static protected const string SWITCH_LIGHT_GLOW     = &quot;dz\\gear\\camping\\data\\battery_charger_light_switch_on.rvmat&quot;;
    static protected const string DEFAULT_MATERIAL      = &quot;dz\\gear\\camping\\data\\battery_charger.rvmat&quot;;
    
    protected const string  ATTACHED_CLIPS_STATES[]         = {SEL_CLIPS_CAR, SEL_CLIPS_TRUCK}; // TO DO: If it&#39;s required by design, add helicopter battery here and register its selection names.
    protected const int     ATTACHED_CLIPS_STATES_COUNT     = 2; // Reffers to this ^ array
    
    
    
    int                         m_BatteryEnergy0To100;
    protected float             m_ChargeEnergyPerSecond;
    
    static protected    float   m_BlinkingStatusLightInterval = 0.4; // How often the lights blink
    ref                 Timer   m_UpdateStatusLightsTimer;
    protected           bool    m_BlinkingStatusLightIsOn = false;  // Status of one blinking light
    
    void BatteryCharger()
    {
        m_ChargeEnergyPerSecond = g_Game.ConfigGetFloat (&quot;CfgVehicles &quot; + GetType() + &quot; ChargeEnergyPerSecond&quot;);
        m_UpdateStatusLightsTimer = new Timer( CALL_CATEGORY_SYSTEM );
        SwitchLightOff();
        RegisterNetSyncVariableInt(&quot;m_BatteryEnergy0To100&quot;);
