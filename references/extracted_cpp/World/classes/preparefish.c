// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/preparefish.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PrepareFish extends PrepareAnimal
{   
    override void Init()
    {
        super.Init();
        
        m_Name = &quot;#STR_gutandprepare0&quot;;
        m_IngredientAddHealth[1]    = -4;       // 0 = do nothing
