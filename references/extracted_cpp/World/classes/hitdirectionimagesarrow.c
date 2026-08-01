// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionimages/hitdirectionimagesarrow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HitDirectionImagesArrow_Static
{
    static ref array<string>    m_ImagePathsLayouts;
    static ref array<string>    m_ImagePathsImages;
    
    static void InitArrays()
    {
        if ( !m_ImagePathsLayouts && !m_ImagePathsImages )
        {
            m_ImagePathsLayouts = new array<string>;
            m_ImagePathsImages = new array<string>;
        }
