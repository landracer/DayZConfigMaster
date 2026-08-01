// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/transmitterbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//TRANSMITTER BASE
class TransmitterBase extends ItemTransmitter
{
    //Sounds
    string SOUND_RADIO_TURNED_ON        = &quot;&quot;;
 
    protected EffectSound m_SoundLoop;
    
    // --- SYSTEM EVENTS
    override void OnStoreSave( ParamsWriteContext ctx )
    {   
        super.OnStoreSave( ctx );
        
        //store tuned frequency
        ctx.Write( GetTunedFrequencyIndex() );
