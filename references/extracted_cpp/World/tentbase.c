// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/tentbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TentBase extends ItemBase
{
    const int OPENING_0 = 1;
    const int OPENING_1 = 2;
    const int OPENING_2 = 4;
    const int OPENING_3 = 8;
    const int OPENING_4 = 16;
    const int OPENING_5 = 32;
    const int OPENING_6 = 64;
    const int OPENING_7 = 128;
    const int OPENING_8 = 256;
    const int OPENING_9 = 512;
    const int OPENING_10 = 1024;
    const int OPENING_11 = 2048;
    const int OPENING_12 = 4096;
    const int OPENING_13 = 8192;
    const int OPENING_14 = 16384;
    const int OPENING_15 = 32768;
    
    static const int PACKED     = 0;
    static const int PITCHED    = 1;
    const float MAX_PLACEMENT_HEIGHT_DIFF = 1.5;
    
    protected int m_State;
    protected int m_StateLocal = -1;
    protected bool m_IsEntrance;
    protected bool m_IsWindow;
    protected bool m_IsToggle;
    protected bool m_IsBeingPacked = false;
    protected int m_OpeningMask = 0;
    protected int m_OpeningMaskLocal = -1;
    
    protected ref map< ref ToggleAnimations, bool> m_ToggleAnimations;
    protected ref array<string> m_ShowAnimationsWhenPitched;
    protected ref array<string> m_ShowAnimationsWhenPacked;
    protected Object            m_ClutterCutter;
    protected CamoNet           m_CamoNet;
    protected vector m_HalfExtents; // The Y value contains a heightoffset and not the halfextent !!!
    
    void TentBase()
    {
        m_ToggleAnimations = new map<ref ToggleAnimations, bool>;
        m_ShowAnimationsWhenPitched = new array<string>;
        m_ShowAnimationsWhenPacked = new array<string>;
 
        m_HalfExtents = vector.Zero;
        RegisterNetSyncVariableInt(&quot;m_State&quot;);
        RegisterNetSyncVariableBool(&quot;m_IsEntrance&quot;);
        RegisterNetSyncVariableBool(&quot;m_IsWindow&quot;);  
        RegisterNetSyncVariableBool(&quot;m_IsToggle&quot;);
        RegisterNetSyncVariableInt(&quot;m_OpeningMask&quot;);
        RegisterNetSyncVariableBool(&quot;m_IsBeingPacked&quot;);
        
        ProcessInvulnerabilityCheck(GetInvulnerabilityTypeString());
