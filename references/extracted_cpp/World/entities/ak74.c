// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/ak74.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AK74_Base : AKM_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Ak74Recoil(this);
