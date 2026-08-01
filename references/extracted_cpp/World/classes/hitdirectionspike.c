// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionspike.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HitDirectionEffectSpike extends HitDirectionEffectBase
{
    override HitDirectionImagesBase GetImageData()
    {
        typename type = HitDirectionImagesSpike;
        HitDirectionImagesSpike data = HitDirectionImagesSpike.Cast(type.Spawn());
        return data;
