// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/winchester70.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Winchester70_Base : BoltActionRifle_InnerMagazine_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Winchester70Recoil(this);
