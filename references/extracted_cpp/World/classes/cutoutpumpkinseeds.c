// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/cutoutpumpkinseeds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CutOutPumpkinSeeds extends CutOutSeeds
{   
    override void Init()
    {
        m_Name = &quot;#STR_prepare0&quot;;
        m_IsInstaRecipe = false;                    // should this recipe be performed instantly without animation
        m_AnimationLength = 1;                      // animation length in relative time units
 
        //conditions
        m_MinDamageIngredient[0]        = -1;       // -1 = disable check
        m_MaxDamageIngredient[0]        = 3;        // -1 = disable check
        m_MinQuantityIngredient[0]      = -1;       // -1 = disable check
        m_MaxQuantityIngredient[0]      = -1;       // -1 = disable check
 
        m_MinDamageIngredient[1]        = -1;
        m_MaxDamageIngredient[1]        = 3;
        m_MinQuantityIngredient[1]      = -1;
        m_MaxQuantityIngredient[1]      = -1;
        
        //----------------------------------------------------------------------------------------------------------------------
        //INGREDIENTS
        //ingredient 1
        InsertIngredient(0,&quot;Pumpkin&quot;);                  // you can insert multiple ingredients this way
 
        m_IngredientAddHealth[0]            = 0;        // 0 = do nothing
        m_IngredientSetHealth[0]            = -1;       // -1 = do nothing
        m_IngredientAddQuantity[0]          = 0;        // 0 = do nothing
        m_IngredientDestroy[0]              = true;     // true = destroy, false = do nothing
 
        //ingredient 2
        InsertIngredient(1,&quot;Sickle&quot;);
        InsertIngredient(1,&quot;KukriKnife&quot;);
        InsertIngredient(1,&quot;FangeKnife&quot;);
        InsertIngredient(1,&quot;Hacksaw&quot;);
        InsertIngredient(1,&quot;KitchenKnife&quot;);
        InsertIngredient(1,&quot;SteakKnife&quot;);
        InsertIngredient(1,&quot;StoneKnife&quot;);
        InsertIngredient(1,&quot;Cleaver&quot;);
        InsertIngredient(1,&quot;CombatKnife&quot;);
        InsertIngredient(1,&quot;HuntingKnife&quot;);
        InsertIngredient(1,&quot;Machete&quot;);
        InsertIngredient(1,&quot;CrudeMachete&quot;);
        InsertIngredient(1,&quot;OrientalMachete&quot;);
        InsertIngredient(1,&quot;AK_Bayonet&quot;);
        InsertIngredient(1,&quot;M9A1_Bayonet&quot;);
        InsertIngredient(1,&quot;Mosin_Bayonet&quot;);
        InsertIngredient(1,&quot;SKS_Bayonet&quot;);
        InsertIngredient(1,&quot;FarmingHoe&quot;);
        InsertIngredient(1,&quot;Shovel&quot;);
        InsertIngredient(1,&quot;Crowbar&quot;);
        InsertIngredient(1,&quot;Hammer&quot;);
        InsertIngredient(1,&quot;Mace&quot;);
        InsertIngredient(1,&quot;Wrench&quot;);
        InsertIngredient(1,&quot;LugWrench&quot;);
        InsertIngredient(1,&quot;Screwdriver&quot;);
        InsertIngredient(1,&quot;SledgeHammer&quot;);
        InsertIngredient(1,&quot;BaseballBat&quot;);
        InsertIngredient(1,&quot;Pickaxe&quot;);
        InsertIngredient(1,&quot;Sword&quot;);
        InsertIngredient(1,&quot;WoodAxe&quot;);
        InsertIngredient(1,&quot;FirefighterAxe&quot;);
        InsertIngredient(1,&quot;Hatchet&quot;);
        InsertIngredient(1,&quot;HandSaw&quot;);
        InsertIngredient(1,&quot;BoneKnife&quot;);
 
        m_IngredientAddHealth[1]        = -2;
        m_IngredientSetHealth[1]        = -1;
        m_IngredientAddQuantity[1]      = 0;
        m_IngredientDestroy[1]          = false;
 
 
        //----------------------------------------------------------------------------------------------------------------------
        //RESULTS
        //result1
        AddResult(&quot;PumpkinSeeds&quot;);              //add results here
 
        m_ResultSetFullQuantity[0]      = false;    // true = set full quantity, false = do nothing
        m_ResultSetQuantity[0]          = 12;       // -1 = do nothing
        m_ResultSetHealth[0]            = -1;       // -1 = do nothing
        m_ResultInheritsHealth[0]       = 0;        // (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
        m_ResultInheritsColor[0]        = -1;       // (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value &quot;color&quot; of ingredient (value)
        m_ResultToInventory[0]          = -2;       // (value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
        m_ResultReplacesIngredient[0]   = -1;       // (value) == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value
    
        //result2
        AddResult(&quot;SlicedPumpkin&quot;);//add results here
 
        m_ResultSetFullQuantity[1]      = true;
        m_ResultSetQuantity[1]          = -1;
        m_ResultSetHealth[1]            = -1;
        m_ResultInheritsHealth[1]       = 0;
        m_ResultInheritsColor[1]        = -1;
        m_ResultToInventory[1]          = 1;
        m_ResultReplacesIngredient[1]   = -1;
 
        //result3
        AddResult(&quot;SlicedPumpkin&quot;);//add results here
 
        m_ResultSetFullQuantity[2]      = true;
        m_ResultSetQuantity[2]          = -1;
        m_ResultSetHealth[2]            = -1;
        m_ResultInheritsHealth[2]       = 0;
        m_ResultInheritsColor[2]        = -1;
        m_ResultToInventory[2]          = 1;
        m_ResultReplacesIngredient[2]   = -1;
