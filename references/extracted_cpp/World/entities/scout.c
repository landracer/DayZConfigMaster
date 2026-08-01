// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/scout.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Scout_Base : BoltActionRifle_ExternalMagazine_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new ScoutRecoil(this);
