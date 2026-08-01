// Source: F:/Games/Dayz/scripts/3_game/modinfo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModInfo
{
    proto owned string GetName();
    proto owned string GetPicture();
    proto owned string GetLogo();
    proto owned string GetLogoSmall();
    proto owned string GetLogoOver(); //hover-over logo variant
    proto owned string GetTooltip();
    proto owned string GetOverview(); //description
    proto owned string GetAction();
    proto owned string GetAuthor();
    proto owned string GetVersion();
    proto bool GetDefault();
    proto bool GetIsDLC();
    proto bool GetIsOwned();
    proto void GoToStore();
    
    static const string DEFAULT_PICTURE = &quot;Gui/textures/modlogo_default_co.edds&quot;;
    static const string DEFAULT_LOGO = &quot;Gui/textures/modlogo_default_co.edds&quot;;
    static const string DEFAULT_LOGO_SMALL = &quot;Gui/textures/modlogo_default_co.edds&quot;;
    static const string DEFAULT_LOGO_OVER = &quot;Gui/textures/modlogo_default_hover_co.edds&quot;;
    static const string DEFAULT_OVERVIEW = &quot;&quot;;
    
    static string GetDLCImage(string name)
    {
        string imagePath;
        switch (name)
        {
            case &quot;badlands&quot;:
            {
                imagePath = &quot;gui/textures/dlc_panel_badlands.edds&quot;;
                break;
            }
            case &quot;frostline&quot;:
            {
                imagePath = &quot;gui/textures/dlc_panel_frostline.edds&quot;;
                break;
            }
            default:
            {
                imagePath = &quot;set:dayz_gui image:ProgressDayZFull&quot;;
                break;
            }
        }
        
        return imagePath;
