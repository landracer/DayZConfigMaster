// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/cleanweapon.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CleanWeapon : RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#repair&quot;;
        m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
        m_AnimationLength = 1.5;//animation length in relative time units
        m_Specialty = -0.02;// value > 0 for roughness, value < 0 for precision
        
        
        //conditions
        m_MinDamageIngredient[0] = -1;//-1 = disable check
        m_MaxDamageIngredient[0] = 3;//-1 = disable check
        
        m_MinQuantityIngredient[0] = -1;//-1 = disable check
        m_MaxQuantityIngredient[0] = -1;//-1 = disable check
        
        m_MinDamageIngredient[1] = 1;//-1 = disable check
        m_MaxDamageIngredient[1] = 3;//-1 = disable check
        
        m_MinQuantityIngredient[1] = -1;//-1 = disable check
        m_MaxQuantityIngredient[1] = -1;//-1 = disable check
        //----------------------------------------------------------------------------------------------------------------------
        
        //INGREDIENTS
        //ingredient 1
        InsertIngredient(0,&quot;WeaponCleaningKit&quot;);//you can insert multiple ingredients this way
        
        m_IngredientAddHealth[0] = 0;// 0 = do nothing
        m_IngredientSetHealth[0] = -1; // -1 = do nothing
        m_IngredientAddQuantity[0] = 0;// 0 = do nothing
        m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
        m_IngredientUseSoftSkills[0] = true;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        
        //ingredient 2
        InsertIngredient(1,&quot;DefaultWeapon&quot;,DayZPlayerConstants.CMD_ACTIONFB_CLEANING_WEAPON, true);
        InsertIngredient(1,&quot;DefaultMagazine&quot;);
        InsertIngredient(1,&quot;ItemSuppressor&quot;);
        
        m_IngredientAddHealth[1] = 0;// 0 = do nothing
        m_IngredientSetHealth[1] = -1; // -1 = do nothing
        m_IngredientAddQuantity[1] = 0;// 0 = do nothing
        m_IngredientDestroy[1] = false;// false = do nothing
        m_IngredientUseSoftSkills[1] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
