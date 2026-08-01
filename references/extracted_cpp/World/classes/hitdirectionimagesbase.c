// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionimages/hitdirectionimagesbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HitDirectionImagesBase
{
    int                         m_ImageIndex;
    
    void HitDirectionImagesBase()
    {
        if ( (!GetTypeLayoutPathArray() && !GetTypeImagePathArray()) )
        {
            PerformRegisterImages();
        }
        
        RandomizeImageIdx();
