// Source: F:/Games/Dayz/scripts/3_game/systems/universaltemperaturesource/universaltemperaturesourcelambdabase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalTemperatureSourceLambdaBase
{
    protected float m_LastExecuteTime = -1;
    protected float m_ExecuteInterval = -1;
    protected bool m_AffectsPlayer;
    
    void UniversalTemperatureSourceLambdaBase()
    {
        m_AffectsPlayer = true;
