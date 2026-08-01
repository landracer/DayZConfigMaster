// Source: F:/Games/Dayz/scripts/4_world/entities/dayzspectator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZSpectator : Camera
{
    private float m_SpeedMultiplier = 1.0;
    
    private float m_SendUpdateAcc = 0.0;
    
    void DayZSpectator()
    {
        SetEventMask(EntityEvent.FRAME);
