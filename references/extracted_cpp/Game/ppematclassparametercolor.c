// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matparameters/ppematclassparametercolor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ColorValuesData : Managed
{
    protected ref array<float> m_Values;
    protected int m_Operator;
    
    void ColorValuesData(array<float> values, int operator)
    {
        m_Values = new array<float>;
        m_Values.Copy(values);
        m_Operator = operator;
