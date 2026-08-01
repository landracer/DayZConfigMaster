// Source: F:/Games/Dayz/scripts/4_world/entities/creatures/infected/dayzinfectedcfgbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------------------------------------------------------------------------
// DayZInfectedTypeCreate - main dayz type config
//-----------------------------------------------------------------------------------------------
 
void DayZInfectedTypeCreate(DayZInfectedType pType)
{
    pType.RegisterAttacks();

    pType.RegisterHitComponentsForAI();
