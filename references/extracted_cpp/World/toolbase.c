// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/toolbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//TODO trees are static objects, there is no script event for playing sounds on clients when they are chopped down.
class ToolBase extends ItemBase
{   
    protected int m_MineDisarmRate = 60; //Success rate when disarming with this tool
    
    void ToolBase()
    {
        
