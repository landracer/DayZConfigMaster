// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/prepareanimal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PrepareAnimal extends RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#skin&quot;;
        m_IsInstaRecipe = false;                    // should this recipe be performed instantly without animation
        m_AnimationLength = 1.5;                    // animation length in relative time units
        m_RecipeUID = DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING;
 
        //----------------------------------------------------------------------------------------------------------------------
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
        InsertIngredient(1,&quot;Sickle&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);                    //you can insert multiple ingredients this way
        InsertIngredient(1,&quot;KukriKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;FangeKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Hacksaw&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;HandSaw&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;KitchenKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;SteakKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;StoneKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Cleaver&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;CombatKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;HuntingKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Machete&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;CrudeMachete&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;OrientalMachete&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;WoodAxe&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Hatchet&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;FirefighterAxe&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Sword&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;AK_Bayonet&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;M9A1_Bayonet&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;SKS_Bayonet&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;BoneKnife&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Screwdriver&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
        InsertIngredient(1,&quot;Iceaxe&quot;, DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true);
 
        m_IngredientAddHealth[1]        = -6;           // 0 = do nothing
        m_IngredientSetHealth[1]        = -1;           // -1 = do nothing
        m_IngredientAddQuantity[1]      = 0;            // 0 = do nothing
        m_IngredientDestroy[1]          = false;        // false = do nothing
