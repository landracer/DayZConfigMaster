// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/gasmask_filter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GasMask_Filter : ItemBase
{
    /*
    override void OnQuantityChanged(float delta)
    {
        super.OnQuantityChanged(delta);
        
        if (GetQuantity() == 0)
        {
            SetHealth(&quot;&quot;,&quot;&quot;,0);
        }
    }
    */
    
    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
    {
        if (g_Game.IsServer())
        {
            if( newLevel == GameConstants.STATE_RUINED )
            {
                SetQuantity(0);
            }
        }
