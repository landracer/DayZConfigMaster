// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/cz527.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CZ527_Base : BoltActionRifle_ExternalMagazine_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new CZ527Recoil(this);
