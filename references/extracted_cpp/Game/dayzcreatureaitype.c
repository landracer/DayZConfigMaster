// Source: F:/Games/Dayz/scripts/3_game/entities/dayzcreatureaitype.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZCreatureAIType : DayZCreatureType
{
    string m_CfgPath;
    string m_AnimEventsCfgPath;
    
    private ref map<int, ref AnimSoundEvent>        m_AnimSoundEvents;
    private ref map<int, ref AnimStepEvent>         m_AnimStepEvents;
    private ref map<int, ref AnimSoundVoiceEvent>   m_AnimSoundVoiceEvents;
    private ref map<int, ref AnimDamageEvent>   m_AnimDamageEvents;
 
    void DayZCreatureAIType()
    {
        m_CfgPath = &quot;CfgVehicles &quot; + GetName() + &quot; &quot;;
        m_AnimEventsCfgPath = m_CfgPath + &quot;AnimEvents &quot;;
        LoadParams();
