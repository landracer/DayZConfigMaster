// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/shippingcontainerkeys_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShippingContainerKeys_ColorBase : ToolBase
{
    override void InitItemSounds()
    {
        super.InitItemSounds();
        
        ItemSoundHandler handler = GetItemSoundHandler();
        handler.AddSound(SoundConstants.ITEM_KEY_BREAK, &quot;AlarmClock_Hit_SoundSet&quot;);
