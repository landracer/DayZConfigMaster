// Source: F:/Games/Dayz/scripts/4_world/classes/toggleselections.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ToggleAnimations
{
    protected string m_ToggleOff;
    protected string m_ToggleOn;
    int m_OpeningBit;
    //ref protected array<string>   m_ToggleOffLinkedAnimations;
    //ref protected array<string>   m_ToggleOnLinkedAnimations;
    
    void ToggleAnimations( string toggle_off, string toggle_on, int mask, array<string> linked_anims_off = null, array<string> linked_anims_on = null )
    {
        m_ToggleOff = toggle_off;
        m_ToggleOn = toggle_on;
        m_OpeningBit = mask;
        //m_ToggleOffLinkedAnimations = linked_anims_off;
        //m_ToggleOnLinkedAnimations = linked_anims_on;
