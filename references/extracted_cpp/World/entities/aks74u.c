// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/automaticrifle/aks74u.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AKS74U : AK74_Base
{
    override RecoilBase SpawnRecoilObject()
    {
        return new Aks74uRecoil(this);
