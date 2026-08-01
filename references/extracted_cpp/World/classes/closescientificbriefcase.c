// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/closescientificbriefcase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CloseScientificBriefcase extends RecipeBase
{   
    override void Init()
    {
        m_Name = &quot;#STR_lock_briefcase&quot;;
        m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
        m_AnimationLength = 1;//animation length in relative time units
        m_Specialty = 0.03;// value > 0 for roughness, value < 0 for precision
        
        //----------------------------------------------------------------------------------------------------------------------
        
        //INGREDIENTS
        //ingredient 1
        InsertIngredient(0,&quot;ScientificBriefcase&quot;);//you can insert multiple ingredients this way
        
        m_IngredientAddHealth[0] = 0;// 0 = do nothing
        m_IngredientSetHealth[0] = -1; // -1 = do nothing
        m_IngredientAddQuantity[0] = 0;// 0 = do nothing
        m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
        m_IngredientUseSoftSkills[0] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        m_MaxDamageIngredient[0] = 3;
        
        //ingredient 2
        InsertIngredient(1,&quot;ScientificBriefcaseKeys&quot;);//you can insert multiple ingredients this way
        
        m_IngredientAddHealth[1] = -2.5;// 0 = do nothing
        m_IngredientSetHealth[1] = -1; // -1 = do nothing
        m_IngredientAddQuantity[1] = 0;// 0 = do nothing
        m_IngredientDestroy[1] = false;// false = do nothing
        m_IngredientUseSoftSkills[1] = false;// set &#39;true&#39; to allow modification of the values by softskills on this ingredient
        m_MaxDamageIngredient[1] = 3;
        
        //----------------------------------------------------------------------------------------------------------------------
