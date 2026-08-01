// Source: F:/Games/Dayz/scripts/5_mission/mission/gameplayeffectwidgets/gewidgetsmetadata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
TODO - doxygen formating
*/

class GameplayEffectsData extends Managed
{
    protected ref array<ref Widget>     m_WidgetArray;
    int                                 m_Type;
    int                                 m_WidgetSetIdentifier;
    Widget                              m_LayoutRoot;
    
    void GameplayEffectsData(array<ref Widget> input, int type, int user_override = -1)
    {
        m_WidgetArray = input;
        m_Type = type;
        m_WidgetSetIdentifier = type;
        
        if (user_override != -1)
        {
            m_WidgetSetIdentifier = user_override;
        }
