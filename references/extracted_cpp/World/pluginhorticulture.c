// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginhorticulture.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginHorticulture extends PluginBase
{
    ref map<string, ref PlantMaterialHealth> m_PlantMaterials;
    
    void PluginHorticulture()
    {
        m_PlantMaterials = new map<string, ref PlantMaterialHealth>;
        
        LoadFromCfg();
