// Source: F:/Games/Dayz/scripts/3_game/effectmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class SEffectManager
{
    protected static ref map<int, ref Effect> m_EffectsMap;
    protected static ref array<int> m_FreeEffectIDs;
    protected static int m_HighestFreeEffectID = 1;
    static const int INVALID_ID = 0;
    protected static bool m_IsCleanup;
    protected static bool m_IsInitialized;
    
    protected static ref map<string, ref SoundParams> m_ParamsMap;
    
    static ref ScriptInvoker Event_OnFrameUpdate;
    
    protected static ref map<int, EffecterBase> m_EffectersMap;
    protected static ref array<int> m_FreeEffecterIDs;
    
    protected static int m_HighestFreeEffecterID = 1;
    
    
    static int PlayInWorld(notnull Effect eff, vector pos)
    {
        // Stop the effect first, just in case
        eff.Stop();
        
        int id = EffectRegister(eff);
        
        eff.SetPosition( pos );
        eff.Start();    
        
        return id;
