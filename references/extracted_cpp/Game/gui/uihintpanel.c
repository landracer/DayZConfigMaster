// Source: F:/Games/Dayz/scripts/3_game/gui/hints/uihintpanel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    Ui class for hints in in-game-menu
*/
 
class UiHintPanel extends ScriptedWidgetEventHandler
{   
    #ifdef DIAG_DEVELOPER
    static int              m_ForcedIndex = -1;//only for debug purposes
    #endif
    
    // Const
    protected int           m_SlideShowDelay            = 25000;                                            // The speed of the slideshow 
    protected string                m_RootPath          = &quot;Gui/layouts/new_ui/hints/in_game_hints.layout&quot;;  // Layout path 
    protected const string          m_DataPath          = &quot;scripts/data/hints.json&quot;;                        // Json path
    // Widgets
    protected Widget                m_RootFrame;
    protected Widget                m_SpacerFrame;
    protected ButtonWidget          m_UiLeftButton;
    protected ButtonWidget          m_UiRightButton;
    protected RichTextWidget        m_UiDescLabel;
    protected TextWidget            m_UiHeadlineLabel;
    protected ImageWidget           m_UiHintImage;
    protected TextWidget            m_UiPageingLabel;
    // Data     
    protected ref array<ref HintPage>   m_ContentList;
    protected int                   m_PageIndex = int.MIN;
    protected DayZGame              m_Game;
    protected bool                  m_Initialized;
    protected Widget                m_ParentWidget;
    protected int                   m_PreviousRandomIndex = int.MIN;
    
    // ---------------------------------------------------------
    
    // Constructor
    void UiHintPanel(Widget parent_widget)
    {       
        DayZGame game = DayZGame.Cast(g_Game);
        m_ParentWidget = parent_widget;
        Init(game);
