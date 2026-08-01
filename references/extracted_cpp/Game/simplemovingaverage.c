// Source: F:/Games/Dayz/scripts/3_game/tools/simplemovingaverage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SimpleMovingAverage<Class T>
{
    private T m_Sum                 = 0;
    private int m_Pointer           = 0;
    private int m_Size              = 0;
    private ref array<T> m_Samples  = new array<T>();

    void SimpleMovingAverage(int pSize, T pDefaultValue = 0)
    {
        m_Size = pSize;
 
        for (int i = 0; i < m_Size; i++)
        {
            m_Samples.Insert(pDefaultValue);
            m_Sum += m_Samples[i];
        }
