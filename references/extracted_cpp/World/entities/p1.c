// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/p1.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class P1_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new P1Recoil(this);
