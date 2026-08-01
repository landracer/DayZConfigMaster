// ======================================================================
// File: simplemovingaverage_8c_source.html
// Category: other
// ======================================================================

class SimpleMovingAverage<Class T>
{
private T m_Sum = 0;
private int m_Pointer = 0;
private int m_Size = 0;
private ref array<T> m_Samples = new array<T>();
void SimpleMovingAverage(int pSize, T pDefaultValue = 0)
{
m_Size = pSize;
for (int i = 0; i < m_Size; i++)
{
m_Samples.Insert(pDefaultValue);
m_Sum += m_Samples[i];
}
}
T Add(T newSample)
{
m_Sum = m_Sum - m_Samples[m_Pointer] + newSample;
m_Samples[m_Pointer++] = newSample;
if (m_Pointer == m_Size)
{
m_Pointer = 0;
}
return m_Sum / m_Size;
}
array<T> GetSamples()
{
return m_Samples;
}
}