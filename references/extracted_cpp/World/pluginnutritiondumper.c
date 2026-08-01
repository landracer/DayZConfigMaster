// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/pluginnutritiondumper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class PluginNutritionDumper extends PluginBase
{
    /*
    ref TStringArray m_AllPaths = new TStringArray;
    ref TStringArray m_AllLines = new TStringArray;
    ref map<string, int> m_ParamPool = new map<string, int>;
    
    string config_path;
    string child_name;
    int scope;
    string path;
    PlayerBase m_Player;
    override void OnInit()
    {
        m_AllPaths.Insert(&quot;CfgVehicles&quot;);
        m_AllPaths.Insert(&quot;cfgLiquidDefinitions&quot;);
        m_Player = PlayerBase.Cast(g_Game.GetPlayer());
    }
    
    void CheckInit()
    {
        m_AllLines.Clear();
        string line = &quot;Classname(stage),energy,water,toxicity,fullnessIndex,nutritionalIndex&quot;;
        m_AllLines.Insert(line);
        
        for(int i = 0; i < m_AllPaths.Count(); i++)
        {
            config_path = m_AllPaths.Get(i);
            int children_count = g_Game.ConfigGetChildrenCount(config_path);
            
            for(int x = 0; x < children_count; x++)
            {
                g_Game.ConfigGetChildName(config_path, x, child_name);
                path = config_path + &quot; &quot; + child_name;
                scope = g_Game.ConfigGetInt( config_path + &quot; &quot; + child_name + &quot; scope&quot; );
                bool should_check = 1;
                if( config_path == &quot;CfgVehicles&quot; && scope == 0)
                {
                    should_check = 0;
                }
                
                if ( should_check )
                {
                    bool has_nutrition = g_Game.ConfigIsExisting(path + &quot; Nutrition&quot;);
                    bool has_stages = g_Game.ConfigIsExisting(path + &quot; Food&quot;);
                    if(has_nutrition || has_stages)
                    {
                        EntityAI item = PlayerBase.Cast(g_Game.GetPlayer()).SpawnEntityOnGroundOnCursorDir(child_name,1);
                        Edible_Base edible = Edible_Base.Cast(item);
                        if(edible)
                        {
                            //Print(&quot;spawning &quot; + child_name);
                            line = &quot;&quot;;
                            NutritionalProfile profile;
                            if(!has_stages)
                            {
                                profile =  edible.GetNutritionalProfile();
                                line = BuildLine(child_name, profile);
                                m_AllLines.Insert(line);
                                //Print(line);
                            }
                            else
                            {
                                for(int z = FoodStageType.RAW; z < FoodStageType.COUNT; z++)
                                {
                                    if( z != FoodStageType.RAW ) 
                                        edible.ChangeFoodStage(z);
                                    profile =  edible.GetNutritionalProfile();
                                    string itemname = child_name + &quot;(stage &quot; + z.ToString()+&quot;)&quot;;
                                    line = BuildLine(itemname, profile);
                                    m_AllLines.Insert(line);
                                }
                            }
                        }
                    }
                }
            }
        }
    
        SaveToFile(&quot;nutritional_values.csv&quot;);
    }
    
    
    protected void SaveToFile(string filename)
    {
        FileHandle file = OpenFile(filename, FileMode.WRITE);
        if( file!=0 )
        {
            for(int i = 0; i < m_AllLines.Count(); i++)
            {
                FPrintln(file,m_AllLines.Get(i));
                Print(m_AllLines.Get(i));
            }
            
        }
 
    }   
    
    string BuildLine(string item_name, NutritionalProfile profile)
    {
        string line = item_name+&quot;,&quot;;
        line = line + profile.GetEnergy()+&quot;,&quot;;
        line = line + profile.GetWaterContent()+&quot;,&quot;;
        line = line + profile.GetToxicity()+&quot;,&quot;;
        line = line + profile.GetFullnessIndex()+&quot;,&quot;;
        line = line + profile.GetNutritionalIndex();
        return line;
    }
    */
}
