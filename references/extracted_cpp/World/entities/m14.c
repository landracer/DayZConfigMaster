// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/m14.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M14_Base : RifleBoltLock_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new M14Recoil(this);
