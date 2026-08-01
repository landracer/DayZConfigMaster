// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/sharpenstick.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SharpenStick extends RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#sharpen&quot;;
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
        InsertIngredient(0,&quot;WoodenStick&quot;);              // you can insert multiple ingredients this way
 
        m_IngredientAddHealth[0]        = 0;            // 0 = do nothing
        m_IngredientSetHealth[0]        = -1;           // -1 = do nothing
        m_IngredientAddQuantity[0]      = 0;            // 0 = do nothing
        m_IngredientDestroy[0]          = false;        // true = destroy, false = do nothing
        
        //ingredient 2
        InsertIngredient(1,&quot;Sickle&quot;);
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
        InsertIngredient(1,&quot;BoneKnife&quot;);
        
        m_IngredientAddHealth[1]        = -4;
        m_IngredientSetHealth[1]        = -1;
        m_IngredientAddQuantity[1]      = 0;
        m_IngredientDestroy[1]          = 0;
 
        //----------------------------------------------------------------------------------------------------------------------
        //RESULTS
        //result1
        AddResult(&quot;Ammo_SharpStick&quot;);
 
        m_ResultSetFullQuantity[0]      = 0;
        m_ResultSetQuantity[0]          = 1;
        m_ResultSetHealth[0]            = -1;
        m_ResultInheritsHealth[0]       = 0;
        m_ResultInheritsColor[0]        = -1;
        m_ResultToInventory[0]          = -2;
        m_ResultReplacesIngredient[0]   = 0;
