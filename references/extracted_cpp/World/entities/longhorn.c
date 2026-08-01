// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/longhorn.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LongHorn_Base : SingleShotPistol_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new LongHornRecoil(this);
