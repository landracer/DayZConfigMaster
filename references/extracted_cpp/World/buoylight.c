// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/buoylight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BuoyLight extends PointLightBase
{
    const string LIGHT_MAT_ON = &quot;DZ\\structures_sakhal\\industrial\\harbour\\data\\Buoy_01_glass_e.rvmat&quot;;
    protected const string LIGHT_MAT_OFF = &quot;DZ\\structures_sakhal\\industrial\\harbour\\data\\Buoy_01_glass.rvmat&quot;;
    protected const int BASE_BRIGTHNESS = 5;
    
    protected bool m_IsOn;
        
    void BuoyLight()
    {
        SetBrightnessTo(BASE_BRIGTHNESS);
        SetRadiusTo(15);
        SetCastShadow(true);
        SetVisibleDuringDaylight(false);
        SetFlareVisible(false);
        SetAmbientColor(1.0, 0.2, 0.2);
        SetDiffuseColor(1.0, 0.2, 0.2);
        
        DimmingConfig dimCfg = new DimmingConfig();             
        dimCfg.AddDimmingPattern({0}, {0.01}, {3}, {0.01}, {1});
        dimCfg.SetPatternQueueRepeat(-1);
        EnableDimming(BASE_BRIGTHNESS, dimCfg);
