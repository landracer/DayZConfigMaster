// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/pokeholesbarrel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PokeHolesBarrel extends RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#STR_PokeHolesBarrel0&quot;;
        m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
        m_AnimationLength = 2;//animation length in relative time units
        m_Specialty = 0.01;// value > 0 for roughness, value < 0 for precision
 
        //conditions
        m_MinDamageIngredient[0] = -1;
        m_MaxDamageIngredient[0] = 3;
        m_MinQuantityIngredient[0] = -1;
        m_MaxQuantityIngredient[0] = -1;
        
        m_MinDamageIngredient[1] = -1;
        m_MaxDamageIngredient[1] = 3;
        m_MinQuantityIngredient[1] = -1;
        m_MaxQuantityIngredient[1] = -1;
        
        
        //ingredient 1
        InsertIngredient(0,&quot;Barrel_ColorBase&quot;);//you can insert multiple ingredients this way
 
        m_IngredientAddHealth[0] = 0;
        m_IngredientAddQuantity[0] = 0;
        m_IngredientSetHealth[0] = -1; // -1 = do nothing
        m_IngredientDestroy[0] = 1;
        m_IngredientUseSoftSkills[1] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        
        //ingredient 2
        InsertIngredient(1,&quot;Sickle&quot;);//you can insert multiple ingredients this way
        InsertIngredient(1,&quot;KukriKnife&quot;);
        InsertIngredient(1,&quot;FangeKnife&quot;);
        InsertIngredient(1,&quot;Hacksaw&quot;);
        InsertIngredient(1,&quot;KitchenKnife&quot;);
        InsertIngredient(1,&quot;SteakKnife&quot;);
        InsertIngredient(1,&quot;HayHook&quot;);
        InsertIngredient(1,&quot;StoneKnife&quot;);
        InsertIngredient(1,&quot;Cleaver&quot;);
        InsertIngredient(1,&quot;CombatKnife&quot;);
        InsertIngredient(1,&quot;HuntingKnife&quot;);
        InsertIngredient(1,&quot;Machete&quot;);
        InsertIngredient(1,&quot;CrudeMachete&quot;);
        InsertIngredient(1,&quot;OrientalMachete&quot;);
        InsertIngredient(1,&quot;Screwdriver&quot;);
        InsertIngredient(1,&quot;Crowbar&quot;);
        InsertIngredient(1,&quot;Pickaxe&quot;);
        InsertIngredient(1,&quot;WoodAxe&quot;);
        InsertIngredient(1,&quot;Hatchet&quot;);
        InsertIngredient(1,&quot;FirefighterAxe&quot;);
        InsertIngredient(1,&quot;Sword&quot;);
        InsertIngredient(1,&quot;AK_Bayonet&quot;);
        InsertIngredient(1,&quot;M9A1_Bayonet&quot;);
        InsertIngredient(1,&quot;Mosin_Bayonet&quot;);
        InsertIngredient(1,&quot;SKS_Bayonet&quot;);
        
        m_IngredientAddHealth[1] = -100;
        m_IngredientSetHealth[1] = -1; // -1 = do nothing
        m_IngredientAddQuantity[1] = 0;
        m_IngredientDestroy[1] = 0;
        m_IngredientUseSoftSkills[1] = true;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        
        // crafting multiple results
        AddResult(&quot;BarrelHoles_&quot;);//additional results, must be processed individually in Do method (as &quot;results&quot; parameter)
        
        m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
        m_ResultSetQuantity[0] = -1;//-1 = do nothing
        m_ResultSetHealth[0] = -1;//-1 = do nothing
        m_ResultInheritsHealth[0] = 0;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
        m_ResultInheritsColor[0] = 0;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value &quot;color&quot; of ingredient (value)
        m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
        m_ResultUseSoftSkills[0] = false;// set &#39;true&#39; to allow modification of the values by softskills on this result
        m_ResultReplacesIngredient[0] = -1;// value == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value
