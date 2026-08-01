// Source: F:/Games/Dayz/scripts/3_game/ppeffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEffects
{
    // COLORIZE IDs
    static const int COLORIZE_NV = 100;
    
    //CONSTANTS
    static const float COLOR_SHOCK = 0.1;//shock color value (relative) //todo
    
    //-------------------------------------------------------
    // BLUR START
    //-------------------------------------------------------
    static int      m_BlurInventory;
    static int      m_BlurDrunk;
    static int      m_BlurFever;
    static int      m_BlurMenu;
    static int      m_BlurOptics;
    static int      m_BlurFlashbang;
    static int      m_BlurShock;
    
    static int      m_BurlapBlindness;
    static int      m_DyingEffect;
    static int      m_ShockEffect;
    
    static int      m_ChromAbbOptic;
    //static int        m_ChromAbbShock;
    
    static int      m_VignetteUnconscious;
    static int      m_VignetteShock;
    static int      m_VignetteTunnel;
    static int      m_VignetteMenu;
    
    static float    m_BloodSaturation;
    
    static ref array<float> m_BlurValues;
    static ref array<float> m_ChromAbbValues;
    static ref array<int> m_VignetteEffects;
    static ref map<int, ref array<float>> m_VignetteValues;
    static ref map<int, ref array<float>> m_ColorValues;
    static ref array<float> m_ColorEffect;
    static ref map<int, ref array<float>> m_ColorizeEffects;
    
    //static float m_UnconsciousVignetteColor[4];
    //static float m_UnconsciousVignetteIntesity;
    
    static float m_ColorValueTotal[4] = {0,0,0,0};
    static float m_ColorOverlayTotal;
 
    static Material m_MatColors;
    static Material m_RadialBlur;
    
    static void Init()
    {
        //Print(&quot;---Init PPEffects---&quot;);
        if ( m_BlurValues )
        {
            delete m_BlurValues;
        }
        if ( m_ChromAbbValues )
        {
            delete m_ChromAbbValues;
        }
        if ( m_VignetteEffects )
        {
            delete m_VignetteEffects;
        }
        if ( m_VignetteValues )
        {
            delete m_VignetteValues;
        }
        if ( m_ColorEffect )
        {
            delete m_ColorEffect;
        }
        
        m_MatColors = g_Game.GetWorld().GetMaterial(&quot;graphics/materials/postprocess/glow&quot;);
        m_RadialBlur = g_Game.GetWorld().GetMaterial(&quot;graphics/materials/postprocess/radialblur&quot;);
        m_BlurValues = new array<float>;
        m_ChromAbbValues = new array<float>;
        m_VignetteEffects = new array<int>;
        m_VignetteValues = new map<int, ref array<float>>;
        
        // add new blur effects here
        m_BlurInventory     = RegisterBlurEffect();
        m_BlurDrunk         = RegisterBlurEffect();
        m_BlurFever         = RegisterBlurEffect();
        m_BlurMenu          = RegisterBlurEffect();
        m_BlurOptics        = RegisterBlurEffect();
        m_BlurFlashbang     = RegisterBlurEffect();
        
        // add chromatic abberation effects here
        m_ChromAbbOptic     = RegisterChromAbbEffect();
        //m_ChromAbbShock   = RegisterChromAbbEffect();
        
        // add vignette effects here
        m_VignetteUnconscious   = RegisterVignetteEffect();
        m_VignetteShock         = RegisterVignetteEffect();
        m_VignetteTunnel        = RegisterVignetteEffect();
        m_VignetteMenu          = RegisterVignetteEffect();
        
        m_ColorEffect = new array<float>;
        m_ColorValues = new map<int, ref array<float>>;
        
        // add new color effects here
        m_BurlapBlindness   = RegisterColorEffect();
        m_DyingEffect       = RegisterColorEffect();
        m_ShockEffect       = RegisterColorEffect();
        
        // ------------------------NV-related stuff below------------------------
        array<float> colorizeDefault = {0.0, 0.0, 0.0};
        m_ColorizeEffects = new map<int, ref array<float>>;
        
        // colorize: r, g, b
        // colorize effects registration
        m_ColorizeEffects.Set(PPEffects.COLORIZE_NV, colorizeDefault);
        //SetNVParams(1.0, 0.0, 2.35, 2.75); //default values
        // ------------------------End of NV-related stuff------------------------
