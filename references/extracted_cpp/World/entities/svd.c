// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/rifle/svd.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SVD_Base : RifleBoltLock_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new SvdRecoil(this);
