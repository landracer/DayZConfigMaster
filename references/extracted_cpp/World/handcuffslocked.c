// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/handcuffslocked.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RestrainingToolLocked extends ItemBase
{
    void ~RestrainingToolLocked()
    {
        PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
        if ( player && player.IsRestrained() )
        {
            player.SetRestrained(false);
        }
