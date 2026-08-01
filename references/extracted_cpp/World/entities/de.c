// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/de.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Deagle_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new DEagleRecoil(this);
