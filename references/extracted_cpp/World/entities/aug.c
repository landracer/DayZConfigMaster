// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/aug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Aug_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new AUGRecoil(this);
