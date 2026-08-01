// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/cz550.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CZ550_Base : BoltActionRifle_ExternalMagazine_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new CZ550Recoil(this);
