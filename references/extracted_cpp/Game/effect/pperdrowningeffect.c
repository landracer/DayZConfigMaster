// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperdrowningeffect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_Drowning extends PPERequester_GameplayBase
{
    private float m_EffectPhase = 0;
    private float m_Magnitude = 0;
    private float m_MinMagnitude = 0.3;
    private float m_MaxMagnitude = 0.3;                                             // Actual Maximum is Min+Max
    private float m_Frequency = 5;
    private float m_Stamina01;
    
    override protected void OnStart(Param par = null)
    {
        super.OnStart(par);
        
        m_EffectPhase = 0;
        m_Magnitude = 0;
