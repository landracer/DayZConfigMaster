// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/armband_white.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
class Armband_White extends Armband_ColorBase
{
    void Armband_White()
    {
        //string color = GetItemVariableString(&quot;varColor&quot;); //SYNCFAIL
 
        if ( color != &quot;&quot; )
        {
            SetObjectTexture (0,color);
            SetObjectTexture (1,color);
            SetObjectTexture (2,color);
        }
    }
}
