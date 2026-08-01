// Source: F:/Games/Dayz/scripts/4_world/entities/building/signs.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Bilboard_Base: House
{
    void Land_Bilboard_Base()
    {
        #ifndef SERVER
        if (g_Game.GetBillboardHandler())
        {
            string texture = g_Game.GetBillboardHandler().GetTextureByType(GetType());
            if (texture)
            {
                int index = GetHiddenSelectionIndex(&quot;advert&quot;);
                if (index != -1)
                    SetObjectTexture(index, texture);   
            }
        }
        #endif
