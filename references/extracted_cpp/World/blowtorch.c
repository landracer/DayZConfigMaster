// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/blowtorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Blowtorch extends ItemBase
{
    const string TEXTURE_FLAME      = &quot;dz\\gear\\cooking\\data\\flame_butane_ca.paa&quot;;
    const string ANIM_PHASE_FLAME   = &quot;FlameHide&quot;;
    const string SOUND_BURNING      = &quot;Blowtorch_Loop_SoundSet&quot;;
    
    protected BlowtorchLight    m_Light;
    
    protected EffectSound       m_SoundBurningLoop;
 
    override void OnWorkStart()
    {
        super.OnWorkStart();
 
        #ifndef SERVER
        m_Light = BlowtorchLight.Cast(ScriptedLightBase.CreateLight(BlowtorchLight, &quot;0 0 0&quot;));
        m_Light.AttachOnMemoryPoint(this, &quot;light&quot;);
        #endif
 
        RefreshFlameVisual(true);
        SoundBurningStart();
