// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/modsmenu/modsmenudetailedentry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModsMenuDetailedEntry extends ScriptedWidgetEventHandler
{
    protected Widget            m_Root;
    protected Widget            m_Detail;
    
    //Header
    protected ImageWidget       m_IconSmall;
    protected ImageWidget       m_IconCollapse;
    protected TextWidget        m_Name;
    
    //Left Side Panel
    protected ImageWidget       m_IconBig;
    protected MultilineTextWidget       m_Author;
    protected TextWidget        m_Version;
    protected RichTextWidget    m_ActionWebsite;
    protected RichTextWidget    m_ActionPurchase;
    
    //Description Panel
    protected RichTextWidget    m_Description;
 
    protected ModInfo           m_Data;
    protected ModsMenuDetailed  m_ParentMenu;
    protected bool              m_IsOpen;
 
    void ModsMenuDetailedEntry(ModInfo data, Widget parent, ModsMenuDetailed parent_menu)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/mods_menu/mods_menu_detailed_entry.layout&quot;, parent);
        m_Detail = m_Root.FindAnyWidget(&quot;DetailContainer&quot;);
        
        m_IconSmall = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;IconSmall&quot;));
        m_IconCollapse = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;collapse_button&quot;));
        m_IconCollapse.LoadImageFile( 1, &quot;set:dayz_gui image:icon_open&quot; );
        m_Name = TextWidget.Cast(m_Root.FindAnyWidget(&quot;Name&quot;));
        
        m_IconBig = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;IconBig&quot;));
        m_Author = MultilineTextWidget.Cast(m_Root.FindAnyWidget(&quot;Author&quot;));
        m_Author.SetLineBreakingOverride(LinebreakOverrideMode.LINEBREAK_WESTERN);
        
        m_Version = TextWidget.Cast(m_Root.FindAnyWidget(&quot;Version&quot;));
        m_ActionWebsite = RichTextWidget.Cast(m_Root.FindAnyWidget(&quot;Link&quot;));
        m_ActionPurchase = RichTextWidget.Cast(m_Root.FindAnyWidget(&quot;Purchase&quot;));
        #ifdef PLATFORM_PS4
        m_ActionPurchase.SetText(&quot;#mod_detail_info_store_PS&quot;);
        #endif
        #ifdef PLATFORM_XBOX
        m_ActionPurchase.SetText(&quot;#mod_detail_info_store_Xbox&quot;);
        #endif
        
        m_Description = RichTextWidget.Cast(m_Root.FindAnyWidget(&quot;Description&quot;));
        
        m_Data = data;
        m_ParentMenu = parent_menu;
        
        m_Root.SetHandler(this);
 
        LoadData();
