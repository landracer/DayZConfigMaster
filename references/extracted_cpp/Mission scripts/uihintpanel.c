// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/hints/uihintpanel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    Ui class for hints in in-game-menu
*/
class UiHintPanel extends ScriptedWidgetEventHandler
{
    // Const
    private const int               m_SlideShowDelay    = 25000;                                                // The speed of the slideshow 
    private const string            m_RootPath          = &quot;Gui/layouts/new_ui/hints/in_game_hints.layout&quot;;      // Layout path 
    private const string            m_DataPath          = &quot;Scripts/data/hints.json&quot;;                            // Json path
    // Widgets
    private Widget                  m_RootFrame;
    private Widget                  m_SpacerFrame;
    private ButtonWidget            m_UiLeftButton;
    private ButtonWidget            m_UiRightButton;
    private RichTextWidget          m_UiDescLabel;
    private TextWidget              m_UiHeadlineLabel;
    private ImageWidget             m_UiHintImage;
    private TextWidget              m_UiPageingLabel;
    // Data     
    private ref array<ref HintPage> m_ContentList;
    private int                     m_PageIndex;
    
    // ---------------------------------------------------------
    
    // Constructor
    void UiHintPanel(Widget parent_widget)
    {       
        // Load Json File 
        LoadContentList();
        // If load successful 
        if (m_ContentList)  
        {
            // Build the layout
            BuildLayout(parent_widget);
            // Get random page index 
            RandomizePageIndex();
            // Populate the layout with data
            PopulateLayout();
            // Start the slideshow
            StartSlideshow();           
        }
        else 
        {
            Print(&quot;ERROR: UiHintPanel - Could not create the hint panel. The data are missing!&quot;);
        }
