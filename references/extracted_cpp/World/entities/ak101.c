// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/ak101.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AK101_Base : AKM_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Ak101Recoil(this);
