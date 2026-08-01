// Source: F:/Games/Dayz/scripts/3_game/canvas.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
typedef bool PIXEL;
class Canvas
{
    int m_SizeX; int m_SizeY;
    
    ref array<ref array<PIXEL>> m_Pixels = new array<ref array<PIXEL>>;
    
    void Canvas(int size_x, int size_y)
    {
        m_SizeX = size_x;
        m_SizeY = size_y;
        for(int i = 0; i < size_y; i++)
        {
            array<PIXEL> x_line = new array<PIXEL>;
            for(int z = 0; z < size_x;z++)
            {
                x_line.Insert(false);
            }
            m_Pixels.Insert(x_line);
            
        }
