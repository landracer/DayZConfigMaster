// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/portablegasstove.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PortableGasStove extends ItemBase
{
    StoveLight  m_Light;
    
    protected const string FLAME_BUTANE_ON          = &quot;dz\\gear\\cooking\\data\\flame_butane_ca.paa&quot;;
    protected const string FLAME_BUTANE_OFF         = &quot;&quot;;
    typename ATTACHMENT_COOKING_POT                 = Pot;
    typename ATTACHMENT_FRYING_PAN                  = FryingPan;
    typename ATTACHMENT_CAULDRON                    = Cauldron;
    
    //cooking
    protected const float PARAM_COOKING_TIME_INC_COEF           = 0.5;      //cooking time increase coeficient, can be used when balancing how fast a food can be cooked
    protected const float PARAM_COOKING_TARGET_TEMP             = 400;      //target temperature for general item heating
    
    private         float m_TimeFactor;
    //
    ref Cooking m_CookingProcess;
    ItemBase m_CookingEquipment;
    
    //sound
    const string SOUND_BURNING      = &quot;portablegasstove_burn_SoundSet&quot;;
    const string SOUND_TURN_ON      = &quot;portablegasstove_turn_on_SoundSet&quot;;
    const string SOUND_TURN_OFF     = &quot;portablegasstove_turn_off_SoundSet&quot;;
    
    protected EffectSound m_SoundBurningLoop;
    protected EffectSound m_SoundTurnOn;
    protected EffectSound m_SoundTurnOff;
    
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaConstant m_UTSLConst;
    
    //cooking equipment
    ItemBase GetCookingEquipment()
    {
        return m_CookingEquipment;
