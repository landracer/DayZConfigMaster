// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/recipes/cutoutseeds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CutOutSeeds extends RecipeBase
{
    override bool CanDo(ItemBase ingredients[], PlayerBase player)
    {
        return !ingredients[0].GetIsFrozen();
