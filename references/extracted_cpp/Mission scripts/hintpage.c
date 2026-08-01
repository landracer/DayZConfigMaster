// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/hints/hintpage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    Data model class for Hint json
    Location: 
*/
class HintPage 
{
    private string  m_Headline;         // Headline hint text   
    private string  m_Description;      // Hint description text
    private string  m_ImagePath;        // Hint image, can be null  
 
    string GetHeadlineText()
    {
        return m_Headline;
