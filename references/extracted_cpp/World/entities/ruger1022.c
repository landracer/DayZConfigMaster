// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/ruger1022.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Ruger1022_Base : RifleBoltFree_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Ruger1022Recoil(this); //TODO
