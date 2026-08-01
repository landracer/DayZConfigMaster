// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/colormanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ColorManager
{
    private ref static ColorManager m_Instance;
    
    static int COLOR_NORMAL_TEXT        = ARGB( 255, 255, 255, 255 );
    static int COLOR_NORMAL_PANEL       = ARGB( 140, 0, 0, 0 );
    static int COLOR_HIGHLIGHT_TEXT     = ARGB( 255, 200, 0, 0 );
    static int COLOR_HIGHLIGHT_PANEL    = ARGB( 255, 0, 0, 0 );
    static int COLOR_SELECTED_TEXT      = ARGB( 255, 255, 255, 255 );
    static int COLOR_SELECTED_PANEL     = ARGB( 255, 200, 0, 0 );
    static int COLOR_DISABLED_TEXT      = ARGB( 100, 255, 255, 255 );
    static int COLOR_DISABLED_PANEL     = ARGB( 140, 0, 0, 0 );
    
    static int ITEM_BACKGROUND_COLOR = ARGB( 50, 255, 255, 255 );
    static int BASE_COLOR = ARGB( 10, 255, 255, 255 );
    static int RED_COLOR = ARGB( 150, 255, 1, 1 );
    static int GREEN_COLOR = ARGB( 150, 1, 255, 1 );
    static int SWAP_COLOR = ARGB( 150, 135, 206, 250 );
    static int FSWAP_COLOR = ARGB( 150, 35, 106, 150 );
    static int COMBINE_COLOR = ARGB( 150, 255, 165, 0 );
    
    void ColorManager()
    {
        m_Instance = this;
