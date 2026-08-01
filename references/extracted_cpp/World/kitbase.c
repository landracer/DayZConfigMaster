// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/kitbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class KitBase extends ItemBase
{
    protected bool                  m_DeployedRegularly;
 
    override bool IsBasebuildingKit()
    {
        return true;
