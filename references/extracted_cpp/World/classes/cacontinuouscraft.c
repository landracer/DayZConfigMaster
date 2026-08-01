// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuouscraft.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousCraft : CAContinuousTime
{       
    override void Setup( ActionData action_data  )
    {
        m_TimeElpased = 0;
        m_AdjustedTimeToComplete = 1000; //indication of arror if somting will be craft 1000 sec
        if ( !m_SpentUnits )
        { 
            m_SpentUnits = new Param1<float>(0);
        }
        else
        {   
            m_SpentUnits.param1 = 0;
        }
        
        WorldCraftActionData action_data_wc = WorldCraftActionData.Cast(action_data);   
        
        PluginRecipesManager module_recipes_manager;
        Class.CastTo(module_recipes_manager, GetPlugin(PluginRecipesManager));
        if( module_recipes_manager )
        {
            m_AdjustedTimeToComplete = module_recipes_manager.GetRecipeLengthInSecs( action_data_wc.m_RecipeID );
            if( module_recipes_manager.GetIsInstaRecipe( action_data_wc.m_RecipeID) || module_recipes_manager.IsEnableDebugCrafting() )
            {
                m_AdjustedTimeToComplete = 0;
            }
            float specialty_weight = module_recipes_manager.GetRecipeSpecialty( action_data_wc.m_RecipeID );
            m_AdjustedTimeToComplete = m_AdjustedTimeToComplete;//removed softskills 
            
            //PrintString(&quot;ttc:&quot; + m_AdjustedTimeToComplete.ToString());
        }
