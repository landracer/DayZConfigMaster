// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int MAX_NUMBER_OF_INGREDIENTS = 2;
const int MAXIMUM_RESULTS = 10;
const float DEFAULT_SPAWN_DISTANCE = 0.6;
 
class RecipeAnimationInfo
{
    string m_IngredientName;
    int m_AnimationUID;
    bool m_ItemVisible;
    
    void RecipeAnimationInfo(string ingredient, int animationID, bool itemVisible)
    {
        m_IngredientName = ingredient;
        m_AnimationUID = animationID;
        m_ItemVisible = itemVisible;
