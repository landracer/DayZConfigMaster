// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/portablegaslamp.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PortableGasLamp extends ItemBase
{
    PortableGasLampLight    m_Light;
    
    private const string GAS_LIGHT_MATERIAL_ON      = &quot;dz\\gear\\cooking\\data\\GasLightOn.rvmat&quot;;
    private const string GAS_LIGHT_MATERIAL_OFF     = &quot;dz\\data\\data\\default.rvmat&quot;;
 
    //sound
    const string SOUND_BURNING      = &quot;portablegaslamp_burn_SoundSet&quot;;
    const string SOUND_TURN_ON      = &quot;portablegaslamp_turn_on_SoundSet&quot;;
    const string SOUND_TURN_OFF     = &quot;portablegaslamp_turn_off_SoundSet&quot;;
    
    protected EffectSound m_SoundBurningLoop;
    protected EffectSound m_SoundTurnOn;
    protected EffectSound m_SoundTurnOff;
    
    //--- POWER EVENTS
    override void OnSwitchOn()
    {
        super.OnSwitchOn();
        
        //sound (client only)
        SoundTurnOn();
