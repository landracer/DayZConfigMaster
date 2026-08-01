// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/mkii.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MKII_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new MkiiRecoil(this);
