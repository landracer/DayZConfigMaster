// Source: F:/Games/Dayz/scripts/3_game/client/mods/modstructure.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModStructure
{
    protected int       m_ModIndex;
    protected string    m_ModPath;
    protected string    m_ModName;
    protected string    m_ModLogo;
    protected string    m_ModLogoSmall;
    protected string    m_ModLogoOver;
    protected string    m_ModActionURL;
    protected string    m_ModTooltip;
    protected string    m_ModOverview;
    
    void ModStructure( int index, string path )
    {
        m_ModIndex  = index;
        m_ModPath   = path;
        LoadData();
