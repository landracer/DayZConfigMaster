// ======================================================================
// File: gp5gasmask_8c_source.html
// Category: other
// ======================================================================

class GP5GasMask extends MaskBase
{
override bool IsObstructingVoice()
{
return true;
}
override int GetVoiceEffect()
{
return VoiceEffectObstruction;
}
}
class GP5GasMask_Filter : ItemBase {};