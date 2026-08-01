// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/sawoffshotgunizh43.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SawoffShotgunIzh43 extends RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#STR_sawoffbarrel0&quot;;
        m_IsInstaRecipe = false;                    // should this recipe be performed instantly without animation
        m_AnimationLength = 2;                      // animation length in relative time units
 
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
        InsertIngredient(0,&quot;Izh43Shotgun&quot;);             // you can insert multiple ingredients this way
 
        m_IngredientAddHealth[0]        = 0;            // 0 = do nothing
        m_IngredientSetHealth[0]        = -1;           // -1 = do nothing
        m_IngredientAddQuantity[0]      = 0;            // 0 = do nothing
        m_IngredientDestroy[0]          = false;        // true = destroy, false = do nothing
 
 
        //ingredient 2
        InsertIngredient(1,&quot;Hacksaw&quot;, DayZPlayerConstants.CMD_ACTIONFB_SPLITTING_FIREWOOD, true);
 
        m_IngredientAddHealth[1]        = -10;
        m_IngredientSetHealth[1]        = -1;
        m_IngredientAddQuantity[1]      = 0;
        m_IngredientDestroy[1]          = false;
 
        //----------------------------------------------------------------------------------------------------------------------
        //RESULTS
