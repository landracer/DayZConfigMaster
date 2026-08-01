// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindayzplayerdebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! PluginDayzPlayerDebugUserData
// *************************************************************************************
class PluginDayzPlayerDebugUserData
{
    void PluginDayzPlayerDebugUserData(string name, int category, int pUserData, bool pFullBody, int pStanceMask = -1)
    {
        m_iUserData     = pUserData;//animation CMD id
        m_bIsFullBody   = pFullBody;
        m_iStanceMask   = pStanceMask;
        m_Category      = category;//pType
        m_Name          = name;
