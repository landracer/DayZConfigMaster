// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/famas.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Famas_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new FamasRecoil(this);
