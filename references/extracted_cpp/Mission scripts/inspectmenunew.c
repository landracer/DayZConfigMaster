// Source: F:/Games/Dayz/scripts/5_mission/gui/inspectmenunew.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//--------------------------------------------------------------------------
class InspectMenuNew extends UIScriptedMenu
{
    private ItemPreviewWidget m_item_widget;
    private ItemPreviewWidget m_slot_widget;
    private int m_characterRotationX;
    private int m_characterRotationY;
    private int m_characterScaleDelta;
    private vector m_characterOrientation;
 
    
    void InspectMenuNew()
    {
        g_Game.GetMission().AddActiveInputExcludes({&quot;inspect&quot;});
