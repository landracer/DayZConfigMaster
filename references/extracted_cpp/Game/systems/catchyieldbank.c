// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchyieldbank.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef map<int,ref YieldItemBase> YieldsMap; //<<typename hash, yield data>> //TODO: why hash? Evaluate
 
class CatchYieldBank
{
    protected ref YieldsMap m_AllYieldsMap;
    private ref array<int> m_OrderedHashes; //for sync purposes
    
    void CatchYieldBank()
    {
        Init();
