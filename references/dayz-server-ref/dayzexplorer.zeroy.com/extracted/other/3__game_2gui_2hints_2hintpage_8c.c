// ======================================================================
// File: 3__game_2gui_2hints_2hintpage_8c_source.html
// Category: other
// ======================================================================

/*
Data model class for Hint json
Location:
*/
class HintPage
{
private string m_Headline; // Headline hint text
private string m_Description; // Hint description text
private string m_ImagePath; // Hint image, can be null
string GetHeadlineText()
{
return m_Headline;
}
string GetDescriptionText()
{
return m_Description;
}
string GetImagePath()
{
return m_ImagePath;
}
}