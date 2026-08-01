// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/pistol/derringer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Derringer_Base : DoubleBarrel_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new DerringerRecoil(this);
