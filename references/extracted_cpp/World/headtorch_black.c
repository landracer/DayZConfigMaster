// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/headtorch_black.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Headtorch_Black : Headtorch_ColorBase 
{
    override void OnLightCreated()
    {
        m_Light.SetColorToRed();
