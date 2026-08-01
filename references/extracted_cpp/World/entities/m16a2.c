// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/m16a2.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M16A2_Base : RifleBoltLock_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new M16A2Recoil(this);
