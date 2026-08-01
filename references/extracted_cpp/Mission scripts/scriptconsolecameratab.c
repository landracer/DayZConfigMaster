// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsolecameratab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleCameraValues
{
    static float m_CurrentFOV;
    static float m_CurrentFocusDistance;
    static float m_CurrentFocusLength;
    static float m_CurrentFocusLengthNear;
    static float m_CurrentBlur;
    static float m_CurrentFocusDepthOffset;
    
    static void SetDefaultValues()
    {
        m_CurrentFOV = 1.0;
        m_CurrentFocusDistance = 0.0;
        m_CurrentFocusLength = -1.0;
        m_CurrentFocusLengthNear = -1.0;
        m_CurrentBlur = 0.0;
        m_CurrentFocusDepthOffset = 1.0;
