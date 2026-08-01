// Source: F:/Games/Dayz/scripts/4_world/classes/recoilbase/recoilbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RecoilBase
{
    bool m_DebugMode;
    
    Weapon_Base m_Weapon;
    PlayerBase m_Player;
    protected bool m_DeleteRequested;
    protected float m_Time;//how much time has elapsed since first update
    protected float m_ReloadTime;//reload time config parameter of the weapon
    protected vector m_RecoilModifier;
    protected bool m_IsClient;
    // for mouse offset
    float m_MouseOffsetRangeMin;//in degrees min
    float m_MouseOffsetRangeMax;//in degrees max
    float m_MouseOffsetRelativeTime = 1;//[0..1] a time it takes to move the mouse the required distance relative to the reload time of the weapon(firing mode)
    float m_HandsOffsetRelativeTime = 1;//[0..1] a time it takes to move the hands the required distance given by the curve relative to the reload time of the weapon(firing mode)
    float m_CamOffsetRelativeTime = 1;//[0..1] a time it takes to move the camera the required distance relative to the reload time of the weapon(firing mode)
    float m_CamOffsetDistance = 0.05;//how far the camera will travel along the z-axis in cm
    float m_MouseOffsetDistance;//how far should the mouse travel
    float m_TimeNormalized;
    //protected float m_MouseOffsetResult;//in degrees max
    protected vector m_MouseOffsetTarget;//move the mouse towards this point
    protected vector m_MouseOffsetTargetAccum;//the overall mouse offset so far(all deltas accumulated)
    protected float m_Angle;//result between the min and max
    // mouse end
    
    protected ref array<vector> m_HandsCurvePoints;
    
    void RecoilBase(Weapon_Base weapon)
    {
        m_Weapon = weapon;
        //m_DebugMode = false;
        m_DebugMode = GetDayZGame().IsAimLogEnabled();
        m_Player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
        m_HandsCurvePoints = new array<vector>;
        Init();
        PostInit(weapon);
