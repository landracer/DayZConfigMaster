// Source: F:/Games/Dayz/scripts/5_mission/gui/staticguiutils.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StaticGUIUtils
{
    static const int IMAGESETGROUP_INVENTORY = 0;
    
    
    static string VerifyIconImageString(int imageset_group = IMAGESETGROUP_INVENTORY, string icon_name = &quot;&quot;)
    {
        if (icon_name == &quot;&quot;)
        {
            return &quot;set:dayz_inventory image:missing&quot;;
        }
        
        if ( !icon_name.Contains(&quot;image:&quot;) )
        {
            switch (imageset_group)
            {
                case IMAGESETGROUP_INVENTORY:
                    return &quot;set:dayz_inventory image:&quot; + icon_name;
            }
            
        }
        return icon_name;
