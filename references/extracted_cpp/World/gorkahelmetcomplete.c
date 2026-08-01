// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/helmetbase/gorkahelmetcomplete.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GorkaHelmet extends HelmetBase
{
    //Debug menu Spawn Ground Special
    override void OnDebugSpawn()
    {
        EntityAI entity;
        if ( Class.CastTo(entity, this) )
        {
            entity.GetInventory().CreateInInventory( &quot;GorkaHelmetVisor&quot; );
        }
