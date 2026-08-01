// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/fal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FAL_Base : RifleBoltLock_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new FALRecoil(this);
