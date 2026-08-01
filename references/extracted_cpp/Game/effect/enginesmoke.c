// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/vehiclesmoke/enginesmoke.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EffEngineSmoke : EffVehicleSmoke
{
    private float m_Time;
    static private float m_UpdateInterval = 2;
    
    void EffEngineSmoke()
    {
        SetEnableEventFrame(true);
