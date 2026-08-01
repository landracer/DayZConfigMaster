// Source: F:/Games/Dayz/scripts/4_world/entities/advancedcommunication/advancedcommunication.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AdvancedCommunication extends EntityAI
{
    static ref map<typename, ref TInputActionMap> m_AdvComTypeActionsMap = new map<typename, ref TInputActionMap>;
    TInputActionMap m_InputActionMap;
    bool    m_ActionsInitialize;
    
    void AdvancedCommunication()
    {
        if (!g_Game.IsDedicatedServer())
        {
            if(g_Game.GetPlayer())
            {
                m_ActionsInitialize = false;
            }
        }   
