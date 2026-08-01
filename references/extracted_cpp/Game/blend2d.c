// Source: F:/Games/Dayz/scripts/3_game/tools/blend2d.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//----------------------------------------------------------------------------------------
/*
    Allows weighted blending of values defined by their 2D position in space.
*/
class Blend2D<Class T>
{
    private ref array<vector> m_Positions;
    private ref array<T>      m_Values;
    private ref array<float>  m_Weights;
 
    //----------------------------------------------------------------------------------------
    void Blend2D()
    {
        m_Positions = {};
        m_Weights   = {};
        m_Values    = {};
