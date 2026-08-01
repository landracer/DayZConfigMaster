// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// I know the filename is stupid, but it&#39;s what the other files in this structure use..
// And I can&#39;t really make it &quot;class Clothing_Base extends Clothing&quot;
// since that will conflict with config and other parts of script and break mods :c
class Clothing extends Clothing_Base
{
    override bool IsClothing()
    {
        return true;
