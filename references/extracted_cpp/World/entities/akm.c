// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/akm.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AKM_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new AkmRecoil(this);
