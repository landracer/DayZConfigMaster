// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowserfavoritestabpc.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserFavoritesTabPc extends ServerBrowserTabPc
{
    // value = ip, name, connection port, query port
    private ref array<ref CachedServerInfo> m_CachedFavoriteServerInfo;
    
    protected override void Construct(Widget parent, ServerBrowserMenuNew menu, TabType type)
    {
        m_CachedFavoriteServerInfo = new array<ref CachedServerInfo>();
        
        super.Construct(parent, menu, type);
