// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowserdetailscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserDetailsContainer extends ScriptedWidgetEventHandler
{   
    protected ServerBrowserTab m_Tab;
 
    protected Widget m_Root;
    protected TextWidget m_ServerNameText;
    protected TextWidget m_ServerTypeResult;
    protected TextWidget m_ServerModeResult;
    protected TextWidget m_LastCharacterResult;
    protected TextWidget m_ServerIP;
    
    #ifdef PLATFORM_WINDOWS
    #ifndef PLATFORM_CONSOLE
    protected GridSpacerWidget m_FriendsContainer;
    protected TextWidget m_FriendsResult;
    #endif
    #endif
    
    protected TextWidget m_TimeAccelerationResult;
    protected ImageWidget m_TimeIcon;
 
    protected TextWidget m_BattleyeResult;
    
    protected WrapSpacerWidget m_VersionContainer;
    protected TextWidget m_VersionResult;
    
    protected GridSpacerWidget m_ServerDecContainer;
    protected MultilineTextWidget m_ServerDesc;
 
    protected WrapSpacerWidget m_DetailsContainerRight;
    protected ImageWidget m_MapImage;
 
    protected ButtonWidget m_BtnShowFilters;
    protected ButtonWidget m_RefreshList;
 
    protected bool m_IsOnline;
 
    void ServerBrowserDetailsContainer(Widget root, ServerBrowserTab parent)
    {
        m_Root = root;
        m_Tab = parent;
        
        m_ServerNameText = TextWidget.Cast(root.FindAnyWidget(&quot;details_servername_label&quot;));
        m_ServerTypeResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_servertype_result_label&quot;));
        m_ServerModeResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_mode_result_label&quot;));
        m_LastCharacterResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_character_result_label&quot;));
        m_ServerIP = TextWidget.Cast(root.FindAnyWidget(&quot;details_ip_result_label&quot;));
        
        #ifdef PLATFORM_WINDOWS
        #ifndef PLATFORM_CONSOLE
        m_FriendsContainer = GridSpacerWidget.Cast(root.FindAnyWidget(&quot;details_friends_container&quot;));
        m_FriendsResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_friends_result_label&quot;));
        #endif
        #endif
        
        m_TimeAccelerationResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_time_result_label&quot;));
        m_TimeIcon = ImageWidget.Cast(root.FindAnyWidget(&quot;detail_time_img&quot;));
        
        m_BattleyeResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_battleye_result_label&quot;));
        
        m_VersionContainer = WrapSpacerWidget.Cast(root.FindAnyWidget(&quot;details_version_container&quot;));
        m_VersionResult = TextWidget.Cast(root.FindAnyWidget(&quot;details_version_result_label&quot;));
        
        m_ServerDecContainer = GridSpacerWidget.Cast(root.FindAnyWidget(&quot;details_description_container&quot;));
        m_ServerDesc = MultilineTextWidget.Cast(root.FindAnyWidget(&quot;details_description_label&quot;));
        
        m_DetailsContainerRight = WrapSpacerWidget.Cast(root.FindAnyWidget(&quot;details_container_right&quot;));
        m_MapImage = ImageWidget.Cast(root.FindAnyWidget(&quot;map_image&quot;));
        
        m_TimeIcon.LoadImageFile(0, &quot;set:dayz_gui image:icon_sun&quot;);
        m_TimeIcon.LoadImageFile(1, &quot;set:dayz_gui image:icon_sun_accel&quot;);
        m_TimeIcon.LoadImageFile(2, &quot;set:dayz_gui image:icon_moon&quot;);
        m_TimeIcon.LoadImageFile(3, &quot;set:dayz_gui image:icon_moon_accel&quot;);
 
        m_BtnShowFilters = ButtonWidget.Cast(parent.GetDetailsRoot().FindAnyWidget(&quot;show_filters_button&quot;));
        m_RefreshList = ButtonWidget.Cast(parent.GetFilterRoot().FindAnyWidget(&quot;refresh_list_button&quot;));
