// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/trapbase/trap_bear.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BearTrap extends TrapBase
{
    static const int RAYCAST_SOURCES_COUNT = 5;
    // Raycasts start positions:
    // Positions are local to model. Vertical offset prevents ground collision.
    static const vector m_RaycastSources[RAYCAST_SOURCES_COUNT] = {
        &quot;0.0 0.1 0.0&quot;,  // center
        &quot;0.2 0.1 0.2&quot;,  // north east
        &quot;-.2 0.1 0.2&quot;,  // north west
        &quot;0.2 0.1 -0.2&quot;, // south east
        &quot;-0.2 0.1 -0.2&quot; // south west
