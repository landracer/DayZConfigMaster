// Source: F:/Games/Dayz/scripts/3_game/ppemanager/pperequesterbank.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPERequesterBank extends Managed
{
    private static ref map<int,ref PPERequesterBase> m_Instances;
    private static bool m_Initialized = false;
    
    static int m_lastID = -1;
    
    static const int INVALID                    = -1;
    static const int REQ_INVENTORYBLUR          = RegisterRequester(PPERequester_InventoryBlur);
    static const int REQ_CONTROLLERDISCONNECT   = RegisterRequester(PPERequester_ControllerDisconnectBlur);
    static const int REQ_GLASSESSPORTBLACK      = RegisterRequester(PPERequester_GlassesSportBlack);
    static const int REQ_GLASSESSPORTBLUE       = RegisterRequester(PPERequester_GlassesSportBlue);
    static const int REQ_GLASSESSPORTGREEN      = RegisterRequester(PPERequester_GlassesSportGreen);
    static const int REQ_GLASSESSPORTORANGE     = RegisterRequester(PPERequester_GlassesSportOrange);
    static const int REQ_GLASSESAVIATOR         = RegisterRequester(PPERequester_GlassesAviator);
    static const int REQ_GLASSESDESIGNER        = RegisterRequester(PPERequester_GlassesDesignerBlack);
    static const int REQ_GLASSESTACTICAL        = RegisterRequester(PPERequester_TacticalGoggles);
    static const int REQ_MOTOHELMETBLACK        = RegisterRequester(PPERequester_MotoHelmetBlack);
    static const int REQ_GLASSESWELDING         = RegisterRequester(PPERequester_WeldingMask);
    static const int REQ_CAMERANV               = RegisterRequester(PPERequester_CameraNV);
    static const int REQ_CAMERAADS              = RegisterRequester(PPERequester_CameraADS);
    static const int REQ_BLOODLOSS              = RegisterRequester(PPERequester_BloodLoss);
    static const int REQ_DEATHEFFECTS           = RegisterRequester(PPERequester_DeathDarkening);
    static const int REQ_UNCONEFFECTS           = RegisterRequester(PPERequester_UnconEffects);
    static const int REQ_TUNELVISSION           = RegisterRequester(PPERequester_TunnelVisionEffects);
    static const int REQ_BURLAPSACK             = RegisterRequester(PPERequester_BurlapSackEffects);
    static const int REQ_INTROCHROMABB          = RegisterRequester(PPERequester_IntroChromAbb);
    static const int REQ_FEVEREFFECTS           = RegisterRequester(PPERequester_FeverEffects);
    static const int REQ_FLASHBANGEFFECTS       = RegisterRequester(PPERequester_FlashbangEffects);
    static const int REQ_SHOCKHITEFFECTS        = RegisterRequester(PPERequester_ShockHitReaction);
    static const int REQ_HEALTHHITEFFECTS       = RegisterRequester(PPERequester_HealthHitReaction);
    static const int REQ_MENUEFFECTS            = RegisterRequester(PPERequester_MenuEffects);
    static const int REQ_CONTROLLERBLUR         = RegisterRequester(PPERequester_ControlsBlur);
    static const int REQ_SERVERBROWSEREFFECTS   = RegisterRequester(PPERequester_ServerBrowserBlur);
    static const int REQ_TUTORIALEFFECTS        = RegisterRequester(PPERequester_TutorialMenu);
    static const int REQ_CONTAMINATEDAREA       = RegisterRequester(PPERequester_ContaminatedAreaTint);
    static const int REQ_SPOOKYAREA             = RegisterRequester(PPERequester_SpookyAreaTint);
    static const int REQ_PAINBLUR               = RegisterRequester(PPERequester_PainBlur);
    static const int REQ_UNDERGROUND            = RegisterRequester(PPERUndergroundAcco);
    static const int REQ_DROWNING               = RegisterRequester(PPERequester_Drowning);
    //static const int REQ_HMP_LVL3             = RegisterRequester(PPERequester_HeavyMetalPoisoning_3);
    static const int REQ_HMP_GHOST              = RegisterRequester(PPERequester_HMPGhosts);
    static const int REQ_LATENCYBLUR            = RegisterRequester(PPERequester_LatencyBlur);
    static const int REQ_FEEDBACKBLUR           = RegisterRequester(PPERequester_FeedbackBlur);
    
    private static ref PPERequesterRegistrations    m_Registrations; //more registrations to be placed here
    
    static void Init()
    {
        m_Registrations = new PPERequesterRegistrations;
        
        if (!m_Instances)
            m_Instances = new map<int,ref PPERequesterBase>;
        
        m_Initialized = true;
