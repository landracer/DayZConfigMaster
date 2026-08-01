// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiongetouttransport.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GetOutTransportActionData : ActionData
{
    Transport m_Vehicle;
    float m_Speed;
    float m_JumpingOutThreshold;
    bool m_WasJumpingOut    = false;
    bool m_WasJumpingOutAnim= false;
    
    // obsolete
    Car m_Car;
    float m_CarSpeed;
    vector m_StartLocation;
    float m_DmgTaken; 
    float m_ShockTaken;
