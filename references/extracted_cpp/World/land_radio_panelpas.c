// Source: F:/Games/Dayz/scripts/4_world/entities/advancedcommunication/land_radio_panelpas.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Radio_PanelPAS extends PASBroadcaster
{
    //Sounds
    const string SOUND_PAS_TURN_ON          = &quot;pastransmitter_turnon_SoundSet&quot;;
    const string SOUND_PAS_TURN_OFF         = &quot;pastransmitter_turnoff_SoundSet&quot;;
    const string SOUND_PAS_TURNED_ON        = &quot;pastransmitter_staticnoise_SoundSet&quot;;
 
    protected EffectSound m_Sound;
    protected EffectSound m_SoundLoop;
    
    //--- BASE
    override bool IsStaticTransmitter()
    {
        return true;
