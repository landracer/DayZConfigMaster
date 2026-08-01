// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cabase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CABase 
{   
    protected ActionBase                m_Action;
    protected float                     m_LastTick;
    protected ref Param                 m_ACData;
    protected ref Param2<float,float>   m_ProgressParam;
    
    void Init( ActionData action_data )
    {
        m_ProgressParam = new Param2<float,float>(0,0);
        m_Action = action_data.m_Action;
        if ( !m_ACData ) 
        {
            m_ACData = new Param;
        }
        m_LastTick = g_Game.GetTime();
        Setup(action_data);
