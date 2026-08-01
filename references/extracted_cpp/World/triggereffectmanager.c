// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/triggereffectmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Used for Effect based triggers ( such as Contaminated areas )
// Allows us to unify these triggers and treat Enter / Exit events as if all triggers were one
class TriggerEffectManager
{
    static ref TriggerEffectManager m_This;
    
    // Player map will only be handled client side as it is most relevant
    ref map<string, ref map<PlayerBase, int>> m_PlayerInsiderMap; // Used to keep track of which players are in which effects
 
    void TriggerEffectManager()
    {
        m_PlayerInsiderMap = new map<string, ref map<PlayerBase, int>>;
