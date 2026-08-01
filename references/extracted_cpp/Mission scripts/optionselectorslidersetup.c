// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectorslidersetup.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelectorSliderSetup extends OptionSelectorBase
{
    protected SliderWidget          m_Slider;
    protected float                 m_MinValue;
    protected float                 m_MaxValue;
    
    void ~OptionSelectorSliderSetup()
    {
        delete m_Root;
