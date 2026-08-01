// ======================================================================
// File: airbornemask_8c_source.html
// Category: other
// ======================================================================

class AirborneMask extends MaskBase
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