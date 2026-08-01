// Source: F:/Games/Dayz/scripts/4_world/classes/arrowmanager/arrowmanagerplayer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ArrowManagerPlayer : ArrowManagerBase
{   
    private static ref map<int,typename> m_TypeHashTable;
    
    void ArrowManagerPlayer(EntityAI owner)
    {
        if (!m_TypeHashTable)
        {
            InitializeHash();
        }
