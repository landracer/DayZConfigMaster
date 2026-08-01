// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionsplash.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HitDirectionEffectSplash extends HitDirectionEffectBase
{
    override HitDirectionImagesBase GetImageData()
    {
        typename type = HitDirectionImagesSplash;
        HitDirectionImagesSplash data = HitDirectionImagesSplash.Cast(type.Spawn());
        return data;
