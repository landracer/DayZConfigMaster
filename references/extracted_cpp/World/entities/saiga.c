// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/shotgun/saiga.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Saiga_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new SiagaRecoil(this);
