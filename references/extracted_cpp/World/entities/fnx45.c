// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/fnx45.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FNX45_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Fnx45Recoil(this);
