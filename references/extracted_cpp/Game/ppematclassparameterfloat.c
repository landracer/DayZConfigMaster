// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matparameters/ppematclassparameterfloat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPEMatClassParameterFloat extends PPEMatClassParameterCommandData
{
    protected ref map<int,ref array<float,int>> m_LayerInfo; //<priority,<value,operator>>
    
    protected PPETemplateDefFloat m_Float;
    protected float m_ValueDefault;
    protected float m_ValueMin;
    protected float m_ValueMax;
    
    void PPEMatClassParameterFloat(int mat_idx, int parameter_idx, PPEClassBase parent)
    {
        m_LayerInfo = new map<int,ref array<float,int>>;
