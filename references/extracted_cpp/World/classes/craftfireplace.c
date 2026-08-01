// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/craftfireplace.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CraftFireplace extends RecipeBase
{
    override void Init()
    {
        m_Name = &quot;#STR_CraftFireplace0&quot;;
        m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
        m_AnimationLength = 1;//animation length in relative time units
        m_Specialty = 0.02;// value > 0 for roughness, value < 0 for precision
        
        
        //conditions
        m_MinDamageIngredient[0] = -1;//-1 = disable check
        m_MaxDamageIngredient[0] = 3;//-1 = disable check
        
        m_MinQuantityIngredient[0] = 1;//-1 = disable check
        m_MaxQuantityIngredient[0] = -1;//-1 = disable check
        
        m_MinDamageIngredient[1] = -1;//-1 = disable check
        m_MaxDamageIngredient[1] = 3;//-1 = disable check
        
        m_MinQuantityIngredient[1] = 1;//-1 = disable check
        m_MaxQuantityIngredient[1] = -1;//-1 = disable check
        //----------------------------------------------------------------------------------------------------------------------
        
        //INGREDIENTS
        //ingredient 1
        InsertIngredient(0,&quot;WoodenStick&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(0,&quot;Firewood&quot; ,           &quot;FireplaceDeploy_firewood&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;Rag&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;BandageDressing&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;Paper&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;ItemBook&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;Bark_Birch&quot;);//you can insert multiple ingredients this way
        InsertIngredient(0,&quot;Bark_Oak&quot;);//you can insert multiple ingredients this way
        
        m_IngredientAddHealth[0] = 0;// 0 = do nothing
        m_IngredientSetHealth[0] = -1; // -1 = do nothing
        m_IngredientAddQuantity[0] = 0;// 0 = do nothing
        m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
        m_IngredientUseSoftSkills[0] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        
        //ingredient 2
        InsertIngredientEx(1,&quot;WoodenStick&quot;,         &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;Firewood&quot; ,           &quot;FireplaceDeploy_firewood&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;Rag&quot;,                 &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;BandageDressing&quot;,     &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;Paper&quot;,               &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;GiftWrapPaper&quot;,       &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;ItemBook&quot;,            &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;Bark_Birch&quot;,          &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;Bark_Oak&quot;,            &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        InsertIngredientEx(1,&quot;EyeMask_ColorBase&quot;,   &quot;FireplaceDeploy&quot;);//you can insert multiple ingredients this way
        
        m_IngredientAddHealth[1] = 0;// 0 = do nothing
        m_IngredientSetHealth[1] = -1; // -1 = do nothing
        m_IngredientAddQuantity[1] = 0;// 0 = do nothing
        m_IngredientDestroy[1] = false;// false = do nothing
        m_IngredientUseSoftSkills[1] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        //----------------------------------------------------------------------------------------------------------------------
        
        //result1
        AddResult(&quot;Fireplace&quot;);//add results here
 
        m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
        m_ResultSetQuantity[0] = -1;//-1 = do nothing
        m_ResultSetHealth[0] = -1;//-1 = do nothing
        m_ResultInheritsHealth[0] = -2;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
        m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value &quot;color&quot; of ingredient (value)
        m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
        m_ResultUseSoftSkills[0] = false;// set &#39;true&#39; to allow modification of the values by softskills on this result
        m_ResultReplacesIngredient[0] = -1;// value == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value
