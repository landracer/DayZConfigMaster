// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercamera1stperson.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! DayZPlayerCamera1stPerson - 1st person camera
// *************************************************************************************
class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{
    static const float  CONST_UD_MIN    = -85.0;        
    static const float  CONST_UD_MAX    = 85.0;         
 
    static const float  CONST_LR_MIN    = -160.0;       
    static const float  CONST_LR_MAX    = 160.0;        
 
 
    void    DayZPlayerCamera1stPerson(DayZPlayer pPlayer, HumanInputController pInput)
    {
        m_iBoneIndex = pPlayer.GetBoneIndexByName(&quot;Head&quot;);
        if (m_iBoneIndex == -1)
        {
            Print(&quot;DayZPlayerCamera1stPerson: main bone not found&quot;);
        }
        
        m_OffsetLS = &quot;0.04 0.04 0&quot;; //x - up, y - forward
