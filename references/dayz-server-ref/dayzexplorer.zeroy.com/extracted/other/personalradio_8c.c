// ======================================================================
// File: personalradio_8c_source.html
// Category: other
// ======================================================================

class PersonalRadio extends TransmitterBase
{
void PersonalRadio()
{
SOUND_RADIO_TURNED_ON = "personalradio_staticnoise_SoundSet";
}
override void OnDebugSpawn()
{
GetInventory().CreateInInventory("Battery9V");
}
}