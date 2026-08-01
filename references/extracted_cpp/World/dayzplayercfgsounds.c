// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercfgsounds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZPlayerTypeStepSoundLookupTableImpl extends DayZPlayerTypeStepSoundLookupTable
{
    void DayZPlayerTypeStepSoundLookupTableImpl()
    {
        m_pSoundTableInstances = new map<int, ref StepSoundLookupTable>;
        m_pSoundTables = new map<int, StepSoundLookupTable>;
        
        string stepsCfgPath = &quot;CfgVehicles SurvivorBase AnimEvents Steps &quot;;
        int stepsCount = g_Game.ConfigGetChildrenCount(stepsCfgPath);
        for(int i = 0; i < stepsCount; i++)
        {
            string stepName;
            g_Game.ConfigGetChildName(stepsCfgPath, i, stepName);
            string stepPath = stepsCfgPath + stepName + &quot; &quot;;
            int id = g_Game.ConfigGetInt(stepPath + &quot;id&quot;);
            
            string tableName;
            g_Game.ConfigGetText(stepPath + &quot;soundLookupTable&quot;, tableName);
 
            
            StepSoundLookupTable table = m_pSoundTableInstances.Get(tableName.Hash());
            if(table == NULL)
            {
                table = new StepSoundLookupTable();
                table.LoadTable(tableName);
                m_pSoundTableInstances.Insert(tableName.Hash(), table);
            }
            m_pSoundTables.Insert(id, table);
        }
