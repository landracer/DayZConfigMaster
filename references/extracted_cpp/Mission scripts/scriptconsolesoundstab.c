// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsolesoundstab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleSoundsTab : ScriptConsoleTabBase
{
    protected bool              m_PlayerPosRefreshBlocked;
    protected static float      DEBUG_MAP_ZOOM = 1;
    protected vector            m_MapPos;
    protected static int        m_SelectedID;
    protected static string     m_SelectedSoundEventType;
    protected static string     m_SelectedAttachments;
    
    protected ButtonWidget      m_SetETSoundButton;
    protected ButtonWidget      m_SetETSoundWeaponButton;
    protected ButtonWidget      m_SetETSoundAttachmentButton;
    protected ButtonWidget      m_SetETSoundVoiceButton;
    protected ButtonWidget      m_PlaySoundEventButton;
    protected ButtonWidget      m_CopySoundset;
    protected ButtonWidget      m_PlaySoundset;
    protected ButtonWidget      m_PlaySoundsetLooped;
    protected ButtonWidget      m_StopSoundset;
 
    protected EditBoxWidget         m_SoundEventIDBox;
    protected EditBoxWidget         m_SoundFilter;
    protected TextListboxWidget     m_SoundsTextListbox;
    
    protected static EffectSound    m_SoundSet;
    protected ref Timer             m_RefreshFilterTimer = new Timer();
    
    protected MapWidget             m_DebugMapWidget;
    protected TextWidget            m_MapDistWidget;
    protected TextWidget            m_MouseCurPos;
    protected TextWidget            m_MapHeadingWidget;
    
    protected ImageWidget           m_BBackgroundSound;
    protected ImageWidget           m_BBackgroundSoundWeapon;
    protected ImageWidget           m_BBackgroundSoundAttachment;
    protected ImageWidget           m_BBackgroundSoundVoice;
    protected ImageWidget           m_SelectedBackground;
    
    protected CheckBoxWidget        m_CheckBoxShoulderAtt;
    protected CheckBoxWidget        m_CheckBoxBodyAtt;
    protected CheckBoxWidget        m_CheckBoxBackAtt;
    
    void ScriptConsoleSoundsTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_MouseCurPos       = TextWidget.Cast(root.FindAnyWidget(&quot;MapSoundsPos&quot;));
        m_MapDistWidget     = TextWidget.Cast(root.FindAnyWidget(&quot;MapSoundsDistance&quot;));
        m_MapHeadingWidget  = TextWidget.Cast(root.FindAnyWidget(&quot;MapHeadingSounds&quot;));
        
        m_SetETSoundButton              = ButtonWidget.Cast(root.FindAnyWidget(&quot;ETSoundButton&quot;));
        m_SetETSoundWeaponButton        = ButtonWidget.Cast(root.FindAnyWidget(&quot;ETSoundWeaponButton&quot;));
        m_SetETSoundAttachmentButton    = ButtonWidget.Cast(root.FindAnyWidget(&quot;ETSoundAttachmentButton&quot;));
        m_SetETSoundVoiceButton         = ButtonWidget.Cast(root.FindAnyWidget(&quot;ETSoundVoiceButton&quot;));
        m_PlaySoundEventButton          = ButtonWidget.Cast(root.FindAnyWidget(&quot;ETSoundPlayButton&quot;));
        m_SoundEventIDBox               = EditBoxWidget.Cast(root.FindAnyWidget(&quot;SoundIDEditBox&quot;));
        m_CheckBoxShoulderAtt           = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;CheckBoxShoulderAtt&quot;));
        m_CheckBoxBodyAtt               = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;CheckBoxBodyAtt&quot;));
        m_CheckBoxBackAtt               = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;CheckBoxBackAtt&quot;));
        m_BBackgroundSound              = ImageWidget.Cast(root.FindAnyWidget(&quot;SoundEventType&quot;));
        m_BBackgroundSoundWeapon        = ImageWidget.Cast(root.FindAnyWidget(&quot;SoundEventType0&quot;));
        m_BBackgroundSoundAttachment    = ImageWidget.Cast(root.FindAnyWidget(&quot;SoundEventType1&quot;));
        m_BBackgroundSoundVoice         = ImageWidget.Cast(root.FindAnyWidget(&quot;SoundEventType2&quot;));
        
        m_CopySoundset      = ButtonWidget.Cast(root.FindAnyWidget(&quot;SoundsetCopy&quot;));
        m_PlaySoundset      = ButtonWidget.Cast(root.FindAnyWidget(&quot;PlaySoundset&quot;));
        m_PlaySoundsetLooped = ButtonWidget.Cast(root.FindAnyWidget(&quot;PlaySoundsetLooped&quot;));
        m_StopSoundset      = ButtonWidget.Cast(root.FindAnyWidget(&quot;StopSoundset&quot;));
        m_DebugMapWidget = MapWidget.Cast(root.FindAnyWidget(&quot;MapSounds&quot;));
 
        m_SoundFilter = EditBoxWidget.Cast(root.FindAnyWidget(&quot;SoundsFilter&quot;));
        m_SoundsTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;SoundsList&quot;));
        
        m_SoundFilter.SetText(m_ConfigDebugProfile.GetSoundsetFilter());
        
        
        ChangeFilterSound();
        
        if (g_Game.GetPlayer())
        {
            m_DebugMapWidget.SetScale(DEBUG_MAP_ZOOM);
            m_DebugMapWidget.SetMapPos(g_Game.GetPlayer().GetWorldPosition());
            SetMapPos(g_Game.GetPlayer().GetWorldPosition());
        }
        
        // reopen update
        m_SoundEventIDBox.SetText(m_SelectedID.ToString());
        
        if (m_SelectedSoundEventType == &quot;Sound&quot;)
            OnClick(m_SetETSoundButton,0,0,0);
        else if (m_SelectedSoundEventType == &quot;SoundWeapon&quot;)
            OnClick(m_SetETSoundWeaponButton,0,0,0);
        else if (m_SelectedSoundEventType == &quot;SoundAttachment&quot;)
            OnClick(m_SetETSoundAttachmentButton,0,0,0);
        else if (m_SelectedSoundEventType == &quot;SoundVoice&quot;)
            OnClick(m_SetETSoundVoiceButton,0,0,0);
        
        string lastSelection = m_SelectedAttachments;
        if (lastSelection.Contains(&quot;shoulder&quot;))
            m_CheckBoxShoulderAtt.SetChecked(true);
        if (lastSelection.Contains(&quot;body&quot;))
            m_CheckBoxBodyAtt.SetChecked(true);
        if (lastSelection.Contains(&quot;back&quot;))
            m_CheckBoxBackAtt.SetChecked(true);
        
