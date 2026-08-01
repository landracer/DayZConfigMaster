// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/defibrillator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Defibrillator extends ItemBase
{
    static const string CHARGING_SOUND =            &quot;defibrillator_charge_SoundSet&quot;;
    static const string CHARGED_AND_READY_SOUND =   &quot;defibrillator_ready_SoundSet&quot;;
    static const string SHOCK_SOUND =               &quot;defibrillator_shock_SoundSet&quot;;
    
    bool m_IsCharged = false;
    
    static float m_ChargeTime = 5;
    static float m_EnergyNeededToCharge = 20; 
    
    ref Timer m_ChargingTimer;
    EffectSound m_ChargedAlarm;
    EffectSound m_ChargingSound;
    
    void Defibrillator()
    {
        // Read all config parameters
        //m_ChargeTime = GetTimeNeededToCharge();
        m_EnergyNeededToCharge = GetEnergyNeededToCharge();
