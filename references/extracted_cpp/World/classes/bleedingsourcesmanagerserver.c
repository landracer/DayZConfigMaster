// Source: F:/Games/Dayz/scripts/4_world/classes/bleedingsources/bleedingsourcesmanagerserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BleedingSourcesManagerServer extends BleedingSourcesManagerBase
{
    const float TICK_INTERVAL_SEC = 3;
    float   m_Tick;
    bool    m_DisableBloodLoss = false;
    ref array<int> m_DeleteList = new array<int>;
    
    protected bool  m_ProcessSourcesRemoval = false; //to avoid constant array counting
    
    const int STORAGE_VERSION = 103;
    
    protected BleedingSourceZone GetBleedingSourceZone(int bit)
    {
        return m_BleedingSourceZone.Get(GetSelectionNameFromBit(bit));
