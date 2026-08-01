// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptscenemanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptSceneManager extends UIPopupScript
{
    private ButtonWidget        m_BtnCancel;
    private ButtonWidget        m_BtnSceneNew;
    private ButtonWidget        m_BtnSceneLoad;
    private ButtonWidget        m_BtnSceneRename;
    private ButtonWidget        m_BtnSceneDuplicate;
    private ButtonWidget        m_BtnSceneDelete;
    private TextListboxWidget   m_LstListScenes;
    
    private static const int m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    private ButtonWidget    m_BtnSave;
    
    private TextWidget      m_TxtWeatherTime;
    
    private SliderWidget    m_SldStartTime;
    private TextWidget      m_TxtStartTimeValue;
    private SliderWidget    m_SldStartDay;
    private TextWidget      m_TxtStartDayValue;
    private SliderWidget    m_SldOvercast;
    private TextWidget      m_TxtOvercastValue;
    private SliderWidget    m_SldRain;
    private TextWidget      m_TxtRainValue;
    private SliderWidget    m_SldFog;
    private TextWidget      m_TxtFogValue;
    private SliderWidget    m_SldWindForce;
    private TextWidget      m_TxtWindForceValue;
    
    private int             m_OrigYear;
    private int             m_OrigMonth;
    private int             m_OrigDay;
    private int             m_OrigHour;
    private int             m_OrigMinute;
    private float           m_OrigOvercast;
    private float           m_OrigRain;
    private float           m_OrigFog;
    private float           m_OrigWindForce;
    
    private int             m_CurrYear;
    private int             m_CurrMonth;
    private int             m_CurrDay;
    private int             m_CurrHour;
    private int             m_CurrMinute;
    private float           m_CurrOvercast;
    private float           m_CurrRain;
    private float           m_CurrFog;
    private float           m_CurrWindForce;
    
    //================================================
    // UIPopupScriptSceneManager
    //================================================  
    void UIPopupScriptSceneManager(Widget wgt)
    {
        m_BtnCancel         = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_cancel&quot;) );
        m_BtnSceneNew       = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_mission_new&quot;) );
        m_BtnSceneLoad      = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_mission_load&quot;) );
        m_BtnSceneRename    = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_mission_rename&quot;) );
        m_BtnSceneDelete    = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_mission_delete&quot;) );
        m_BtnSceneDuplicate = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sm_mission_duplicate&quot;) );
        
        m_LstListScenes = TextListboxWidget.Cast( wgt.FindAnyWidget(&quot;tls_ppp_sm_scene_list&quot;) );
        
        m_BtnSave           = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_st_save&quot;) );
        
        m_TxtWeatherTime    = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_w_time_value&quot;) );
        
        m_SldStartTime      = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_start_time&quot;) );
        m_TxtStartTimeValue = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_start_time_value&quot;) );
        
        m_SldStartDay       = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_start_day&quot;) );
        m_TxtStartDayValue  = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_start_day_value&quot;) );
        
        m_SldOvercast       = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_overcast&quot;) );
        m_TxtOvercastValue  = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_overcast_value&quot;) );
        
        m_SldRain           = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_rain&quot;) );
        m_TxtRainValue      = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_rain_value&quot;) );
        
        m_SldFog            = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_fog&quot;) );
        m_TxtFogValue       = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_fog_value&quot;) );
        
        m_SldWindForce      = SliderWidget.Cast( wgt.FindAnyWidget(&quot;sld_ppp_st_wind_force&quot;) );
        m_TxtWindForceValue = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_st_wind_force_value&quot;) );
