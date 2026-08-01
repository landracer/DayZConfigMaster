// Source: F:/Games/Dayz/scripts/3_game/tools/simplecircularbuffer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SimpleCircularBuffer<Class T>
{
    private int m_Pointer           = 0;
    private int m_Size              = 0;
    private ref array<T> m_Samples  = new array<T>();

    void SimpleCircularBuffer(int pSize, T pDefaultValue)
    {
        m_Size = pSize;
 
        for (int i = 0; i < m_Size; ++i)
            m_Samples.Insert(pDefaultValue);
