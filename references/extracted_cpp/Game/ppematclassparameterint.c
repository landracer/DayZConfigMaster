// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matparameters/ppematclassparameterint.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPEMatClassParameterInt extends PPEMatClassParameterCommandData
{
    protected ref map<int,ref array<int,int>> m_LayerInfo; //<priority,<value,operator>>
    
    protected PPETemplateDefInt m_Int;
    protected int m_ValueDefault;
    
    void PPEMatClassParameterInt(int mat_idx, int parameter_idx, PPEClassBase parent)
    {
        m_LayerInfo = new map<int,ref array<int,int>>;
