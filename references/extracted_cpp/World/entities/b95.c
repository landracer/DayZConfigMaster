// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/b95.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class B95_base : DoubleBarrel_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new B95Recoil(this);
