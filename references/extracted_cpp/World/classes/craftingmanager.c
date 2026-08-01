// Source: F:/Games/Dayz/scripts/4_world/classes/craftingmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class CraftingManager
{
    const int CM_MODE_NONE = 0;
    const int CM_MODE_WORLD = 1;
    const int CM_MODE_INVENTORY = 2;
 
    PlayerBase m_player;
    PluginRecipesManager m_recipesManager; 
    ActionVariantManager m_actionVariantManager;
    int m_recipeID;
    int m_contextualRecipeID;
    int m_recipeCount;
    int m_craftingMode;
    ItemBase m_item1;
    ItemBase m_item2;
    
    ref array<int> m_recipes;
 
    void CraftingManager(PlayerBase player, PluginRecipesManager recipesManager)
    {
        m_recipesManager = recipesManager;
        m_player = player;
        m_craftingMode = CM_MODE_NONE;
        m_actionVariantManager = ActionManagerClient.GetVariantManager( ActionWorldCraft );
        m_actionVariantManager.GetOnUpdateInvoker().Clear();
        m_actionVariantManager.GetOnUpdateInvoker().Insert(OnUpdate);
        m_recipes = new array<int>;
