// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/makarov.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MakarovIJ70_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new MakarovRecoil(this);
