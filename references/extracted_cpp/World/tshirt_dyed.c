// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/tshirt_dyed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
class TShirt_Dyed : Clothing
{
    void TShirt_Dyed()
    {
        string color = GetItemVariableString(&quot;varColor&quot;); //SYNCFAIL
        if ( color != &quot;&quot; )
        {
            SetObjectTexture (0,color);
            SetObjectTexture (1,color);
            SetObjectTexture (2,color);
        }
    }
    
    override bool IsClothing()
    {
        return true;
    }
}
