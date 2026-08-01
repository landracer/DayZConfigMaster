// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/helmetbase/dirtbikehelmet_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DirtBikeHelmet_ColorBase extends HelmetBase
{
    //Debug menu Spawn Ground Special
    override void OnDebugSpawn()
    {
        EntityAI entity;
        if ( Class.CastTo(entity, this) )
        {
            entity.GetInventory().CreateInInventory( &quot;DirtBikeHelmet_Visor&quot; );
            entity.GetInventory().CreateInInventory( &quot;DirtBikeHelmet_Mouthguard&quot; );
        }
