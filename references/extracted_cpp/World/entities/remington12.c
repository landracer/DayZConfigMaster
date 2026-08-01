// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/shotgun/remington12.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class R12_Base : ChamberFirst_InnerMagazine_SemiAutomatic_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new R12Recoil(this);
