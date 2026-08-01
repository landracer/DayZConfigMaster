// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindiagmenu/plugindiagmenumodding.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
#ifdef DOXYGEN
#ifdef MODDING_TEST
#ifdef DIAG_DEVELOPER

#ifdef DOXYGEN
class PluginDiagMenuModded // Just to not have it show up for the regular entry... Doxygen doesn&#39;t know about modding
#else
modded class PluginDiagMenu
#endif
{   
    protected string m_ModdedDiagsExampleRootMenu = &quot;BI - DiagsModdingExample&quot;;
    
    protected string m_ModdedDiagsExampleSubMenu = &quot;Example Sub Menu&quot;;
    
    protected int m_ModdedDiagsExampleRootMenuID;
        protected int m_ModdedDiagsExampleBoolID;
        protected int m_ModdedDiagsExampleSubMenuID;
            protected int m_ModdedDiagsExampleRangeID;
    
    //---------------------------------------------
    override protected void RegisterModdedDiagsIDs()
    {
        super.RegisterModdedDiagsIDs();
        
        m_ModdedDiagsExampleRootMenuID = GetModdedDiagID();
            m_ModdedDiagsExampleBoolID = GetModdedDiagID();
            m_ModdedDiagsExampleSubMenuID = GetModdedDiagID();
                m_ModdedDiagsExampleRangeID = GetModdedDiagID();
    }
    
    //---------------------------------------------
    override protected void RegisterModdedDiags()
    {
        super.RegisterModdedDiags();
        
        // Register the root menu of your mod under the ModdedRootMenu
        // Then register all following menus and items under this menu
        // This can not be enforced, but it will help keep things clean
        // Including being able to easily identify where a debug is coming from
        // So that reports of a broken debug can be sent to the correct developer
        //
        // If you have multiple mods, you might even want to consider to create a root menu with your developer name
        // And then put the mod menus as a submenu
        // To prevent someone running a lot of mods from having an overflooded menu     
        // DiagMenu.MenuExists(...) could serve to help to identify if the root menu already exists when using this format
        // So that the multiple mods can know if they still have to register your root menu or not
        DiagMenu.RegisterMenu(m_ModdedDiagsExampleRootMenuID, m_ModdedDiagsExampleRootMenu, GetModdedRootMenu());
        {
            DiagMenu.RegisterBool(m_ModdedDiagsExampleBoolID, &quot;&quot;, &quot;Modded Example Bool&quot;, m_ModdedDiagsExampleRootMenuID);
            
            // A sub menu inside the root of the mod menu
            // The curly braces are simply for readability
            DiagMenu.RegisterMenu(m_ModdedDiagsExampleSubMenuID, m_ModdedDiagsExampleSubMenu, m_ModdedDiagsExampleRootMenuID);
            {
                DiagMenu.RegisterRange(m_ModdedDiagsExampleRangeID, &quot;&quot;, &quot;Modded Example Range&quot;, m_ModdedDiagsExampleSubMenuID, &quot;3 9 6 3&quot;);
            }
        }
    }
}
 
#ifdef DOXYGEN
class DummyDoxygenClass
{
    int m_IgnoreThisVariable;
}
#endif

#ifdef DOXYGEN
class PluginDiagMenuClientModded // Just to not have it show up for the regular entry... Doxygen doesn&#39;t know about modding
#else
modded class PluginDiagMenuClient
#endif
{
    override protected void BindCallbacks()
    {
        super.BindCallbacks();
 
        DiagMenu.BindCallback(m_ModdedDiagsExampleBoolID, CBModdedDiadIDsExampleBool);
        DiagMenu.BindCallback(m_ModdedDiagsExampleRangeID, CBModdedDiadIDsExampleRange);
    }
    
    static void CBModdedDiadIDsExampleBool(bool enabled)
    {
        Print(&quot;CBModdedDiadIDsExampleBool: &quot; + enabled);
    }
    
    static void CBModdedDiadIDsExampleRange(float value)
    {
        Print(&quot;CBModdedDiadIDsExampleRange: &quot; + value);
    }
}
 
#endif
#endif
#endif

