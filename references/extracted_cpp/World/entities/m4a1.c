// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/m4a1.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M4A1_Base : RifleBoltLock_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new M4a1Recoil(this);
