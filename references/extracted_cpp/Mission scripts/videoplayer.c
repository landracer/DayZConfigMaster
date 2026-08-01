// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/videoplayer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VideoPlayer extends ScriptedWidgetEventHandler
{
    protected Widget        m_Root;
    
    protected ButtonWidget  m_PlayButton;
    protected ButtonWidget  m_PauseButton;
    
    protected ButtonWidget  m_StopButton;
    
    protected ButtonWidget  m_OnceButton;
    protected ButtonWidget  m_RepeatButton;
    
    protected ButtonWidget      m_LoadButton;
    protected GridSpacerWidget  m_LoadVideo;
    
    protected SliderWidget  m_Progress;
    
    protected TextWidget    m_CurrentTime;
    protected TextWidget    m_TotalTime;
    
    protected ImageWidget       m_Buffering;
    /*protected*/ VideoWidget   m_VideoWidget;
    
    void VideoPlayer(Widget parent)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/video_player.layout&quot;, parent);
        m_Root.SetHandler(this);
        m_Root.SetSort(333);
        Init();
