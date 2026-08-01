// Source: F:/Games/Dayz/scripts/4_world/classes/quickbarbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int MAX_QUICKBAR_SLOTS_COUNT = 10;
// Script File
class QuickBarItem
{
    EntityAI m_entity;
    bool m_enabled;
    
    void QuickBarItem()
    {
        m_entity = NULL;
        m_enabled = false;
