// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matparameters/ppematclassparameterbool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPEMatClassParameterBool extends PPEMatClassParameterCommandData
{
    protected ref map<int,ref array<bool,int>> m_LayerInfo; //<priority,<value,operator>>
    
    protected PPETemplateDefBool m_Bool;
    protected bool m_ValueDefault;
    
    void PPEMatClassParameterBool(int mat_idx, int parameter_idx, PPEClassBase parent)
    {
        m_LayerInfo = new map<int,ref array<bool,int>>;
