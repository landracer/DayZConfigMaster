// Source: F:/Games/Dayz/scripts/3_game/gui/spacers/noticespacer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class NoticeSpacer : AutoHeightSpacer
{
    override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
    {
        Update();
        
        return super.OnEvent(eventType, target, parameter0, parameter1);
