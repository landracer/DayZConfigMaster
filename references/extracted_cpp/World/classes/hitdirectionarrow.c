// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionarrow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HitDirectionEffectArrow extends HitDirectionEffectBase
{
    override HitDirectionImagesBase GetImageData()
    {
        typename type = HitDirectionImagesArrow;
        HitDirectionImagesArrow data = HitDirectionImagesArrow.Cast(type.Spawn());
        return data;
