// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/refridgerator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Refridgerator extends ItemBase
{   
    SoundOnVehicle  m_SoundLoopEntity;
    
    override bool IsElectricAppliance()
    {
        return true;
