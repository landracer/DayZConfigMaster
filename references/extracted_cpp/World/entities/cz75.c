// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/cz75.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CZ75_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Cz75Recoil(this);
