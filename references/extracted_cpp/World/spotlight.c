// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/spotlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Spotlight extends ItemBase
{
    private bool    m_IsFolded;
    private bool    m_EvaluateDeployment;
    SpotlightLight  m_Light;
    
    static vector   m_LightLocalPosition;
    static vector   m_LightLocalOrientation = &quot;0 0 0&quot;;
    
    // Spotlight can be extended and compressed
    static const string SEL_REFLECTOR_COMP_U        = &quot;reflector&quot;;
    static const string SEL_CORD_FOLDED_U           = &quot;cord_folded&quot;;
    static const string SEL_CORD_PLUGGED_U          = &quot;cord_plugged&quot;;
    static const string SEL_CORD_PLUGGED_F          = &quot;spotlight_folded_cord_plugged&quot;;
    static const string SEL_CORD_FOLDED_F           = &quot;spotlight_folded_cord_folded&quot;;
    
    static const string SEL_INVENTORY               = &quot;inventory&quot;;
    static const string SEL_PLACING                 = &quot;placing&quot;;
    static const string SEL_GLASS_F                 = &quot;glass_folded&quot;;
    static const string SEL_GLASS_U                 = &quot;glass_unfolded&quot;;
    static const string SEL_REFLECTOR_F             = &quot;reflector_folded&quot;;
    static const string SEL_REFLECTOR_U             = &quot;reflector_unfolded&quot;;
    
    static const int    ID_GLASS_UNFOLDED           = 3;
    static const int    ID_REFLECTOR_UNFOLDED       = 4;
    static const int    ID_GLASS_FOLDED             = 5;
    static const int    ID_REFLECTOR_FOLDED         = 6;
    
    static string       LIGHT_OFF_GLASS             = &quot;dz\\gear\\camping\\Data\\spotlight_glass.rvmat&quot;;
    static string       LIGHT_OFF_REFLECTOR         = &quot;dz\\gear\\camping\\Data\\spotlight.rvmat&quot;;
    static string       LIGHT_ON_GLASS              = &quot;dz\\gear\\camping\\Data\\spotlight_glass_on.rvmat&quot;;
    static string       LIGHT_ON_REFLECTOR          = &quot;dz\\gear\\camping\\Data\\spotlight_glass_on.rvmat&quot;;
    
    //sound
    const string                SOUND_TURN_ON       = &quot;spotlight_turn_on_SoundSet&quot;;
    const string                SOUND_TURN_OFF      = &quot;spotlight_turn_off_SoundSet&quot;;
    
    protected EffectSound   m_SoundTurnOn;
    protected EffectSound   m_SoundTurnOff;
 
    //Spotlight, folded and unfolded
    void Spotlight()
    {
        m_EvaluateDeployment = false;
 
        RegisterNetSyncVariableBool(&quot;m_IsFolded&quot;);
