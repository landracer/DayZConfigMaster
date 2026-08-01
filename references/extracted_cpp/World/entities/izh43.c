// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/shotgun/izh43.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Izh43Shotgun_Base : DoubleBarrel_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Izh43Recoil(this);
