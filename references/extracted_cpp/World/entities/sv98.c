// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/sv98.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SV98_Base : BoltActionRifle_ExternalMagazine_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new SV98Recoil(this);
