// Source: F:/Games/Dayz/scripts/3_game/tools/component/bodystaging.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------
// BODY STAGING
//-----------------------------
/*
Every dead body will have this component which will control its various states.
Read &quot;Dead bodies staging&quot; design on Confluence.
*/
 
class ComponentBodyStaging : Component
{
    bool m_IsSkinned;
    
    void ComponentBodyStaging()
    {
        m_IsSkinned = false;
