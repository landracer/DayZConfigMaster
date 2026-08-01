// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/smg/vss.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VSS_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new VSSRecoil(this);
