// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/skigoggles_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SkiGoggles_ColorBase : Glasses_Base {};
class SkiGoggles_WhiteClear : SkiGoggles_ColorBase{};
class SkiGoggles_WhiteDark : SkiGoggles_ColorBase
{
    override int GetGlassesEffectID()
    {
        return PPERequesterBank.REQ_GLASSESSPORTBLACK;
