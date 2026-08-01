// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowserentry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserEntry extends ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected Widget m_Favorite;
 
    //Basic info
    protected TextWidget m_ServerName;
    protected TextWidget m_ServerPopulation;
    protected TextWidget m_ServerSlots;
    protected TextWidget m_ServerPing;
    protected ImageWidget m_ServerTime; // not currently displayed
    protected ImageWidget m_ServerLock;
    protected ImageWidget m_ServerModIcon;
    protected ImageWidget m_ServerMaKIcon;
 
    //Detailed info
    protected TextWidget m_ServerShard;
    protected TextWidget m_ServerCharacterAlive;
    protected TextWidget m_ServerFriends;
    protected TextWidget m_ServerMode;
    protected TextWidget m_ServerBattleye;
    protected TextWidget m_ServerIP;
    protected TextWidget m_ServerAcceleration;
    protected TextWidget m_ServerMap;
    protected TextWidget m_ServerMods;
    protected ButtonWidget m_ServerModsExpand;
    protected ref array<string> m_Mods;
    
    protected Widget m_DetailedInfo;
    protected bool m_IsExpanded;
    protected bool m_IsFavorited;
    protected bool m_IsOnline;
 
    protected ref GetServersResultRow m_ServerData;
    protected int m_Index;
    protected ServerBrowserTab m_Tab;
    protected bool m_Selected;
    protected bool m_FirstExpand = true;
 
    void ServerBrowserEntry(Widget parent, int index, ServerBrowserTab tab)
    {
    #ifdef PLATFORM_CONSOLE
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/xbox/server_browser_list_entry.layout&quot;, parent);
    #else
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/pc/server_browser_list_entry_pages.layout&quot;, parent);
    #endif
        
        m_Root.Enable(true);
        m_Favorite = m_Root.FindAnyWidget(&quot;favorite_button&quot;);
        m_ServerName = TextWidget.Cast(m_Root.FindAnyWidget(&quot;server_name&quot;));
        m_ServerPopulation = TextWidget.Cast( m_Root.FindAnyWidget(&quot;server_population&quot;));
        m_ServerSlots = TextWidget.Cast(m_Root.FindAnyWidget(&quot;server_slots&quot;));
        m_ServerPing = TextWidget.Cast(m_Root.FindAnyWidget(&quot;server_ping&quot;));
        m_ServerTime = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;server_time&quot;));
        m_ServerLock = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;lock_icon&quot;));
        m_ServerModIcon = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;modded_icon&quot;));
        m_ServerMaKIcon = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;mandk_icon&quot;));
        
        m_ServerShard = TextWidget.Cast(m_Root.FindAnyWidget(&quot;shard_text&quot;));
        m_ServerCharacterAlive = TextWidget.Cast(m_Root.FindAnyWidget(&quot;character_alive_text&quot;));
        m_ServerFriends = TextWidget.Cast(m_Root.FindAnyWidget(&quot;steam_friends_text&quot;));
        m_ServerMode = TextWidget.Cast(m_Root.FindAnyWidget(&quot;mode_text&quot;));
        m_ServerBattleye = TextWidget.Cast(m_Root.FindAnyWidget(&quot;battlleye_text&quot;));
        m_ServerIP = TextWidget.Cast(m_Root.FindAnyWidget(&quot;ip_text&quot;));
        m_ServerAcceleration = TextWidget.Cast(m_Root.FindAnyWidget(&quot;server_acceleration_text&quot;));
        m_ServerMap = TextWidget.Cast(m_Root.FindAnyWidget(&quot;server_map&quot;));
        m_ServerMods = TextWidget.Cast(m_Root.FindAnyWidget(&quot;mods_text&quot;));
        m_ServerModsExpand = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;mods_expand&quot;));
 
        m_DetailedInfo = m_Root.FindAnyWidget(&quot;detailed_info&quot;);
        
        m_Root.FindAnyWidget(&quot;basic_info&quot;).Show(true);
        m_Root.FindAnyWidget(&quot;favorite_image&quot;).Update();
        m_Root.FindAnyWidget(&quot;unfavorite_image&quot;).Update();
        
        m_Index = index;
        m_Tab = tab;
        m_IsOnline = true;
        
        m_ServerTime.LoadImageFile(0, &quot;set:dayz_gui image:icon_sun&quot;);
        m_ServerTime.LoadImageFile(1, &quot;set:dayz_gui image:icon_sun_accel&quot;);
        m_ServerTime.LoadImageFile(2, &quot;set:dayz_gui image:icon_moon&quot;);
        m_ServerTime.LoadImageFile(3, &quot;set:dayz_gui image:icon_moon_accel&quot;);
        
        m_Root.SetHandler(this);
