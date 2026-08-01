// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/1911.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Colt1911_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Colt1911Recoil(this);
