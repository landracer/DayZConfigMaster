// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/glock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Glock19_Base : Pistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new GlockRecoil(this);
