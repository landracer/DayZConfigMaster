// ======================================================================
// File: simplecircularbuffer_8c_source.html
// Category: other
// ======================================================================

class SimpleCircularBuffer<Class T>
{
private int m_Pointer = 0;
private int m_Size = 0;
private ref array<T> m_Samples = new array<T>();
void SimpleCircularBuffer(int pSize, T pDefaultValue)
{
m_Size = pSize;
for (int i = 0; i < m_Size; ++i)
m_Samples.Insert(pDefaultValue);
}
void Add(T newSample)
{
m_Samples[m_Pointer++] = newSample;
if (m_Pointer == m_Size)
m_Pointer = 0;
}
T Get(int pIndex)
{
return m_Samples[pIndex];
}
array<T> GetValues()
{
return m_Samples;
}
}