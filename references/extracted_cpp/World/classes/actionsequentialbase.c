// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/actionsequentialbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SequentialActionData : ActionData
{
    int m_Stage;
    void MultiStageBaseActionData()
    {
        m_Stage = 0;
