// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperflashbangeffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_FlashbangEffects extends PPERequester_GameplayBase
{
    const float VAL_FACTOR = 0.85;
    const float EXPOSURE_MAX = 50;
    protected float m_Exposure;
    protected float m_Intensity;
    
    override protected void OnStart(Param par = null)
    {
        if (!m_IsRunning)
        {
            if (g_Game.GetMission() && g_Game.GetMission().GetEffectWidgets())
                g_Game.GetMission().GetEffectWidgets().AddActiveEffects({EffectWidgetsTypes.COVER_FLASHBANG});
        }
        
        super.OnStart();
