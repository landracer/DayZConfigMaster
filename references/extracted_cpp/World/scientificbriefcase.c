// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/scientificbriefcase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScientificBriefcase : Container_Base
{
    protected ref OpenableBehaviour m_Openable;
    
    override void InitItemVariables()
    {
        super.InitItemVariables();
        
        m_Openable = new OpenableBehaviour(false);
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
