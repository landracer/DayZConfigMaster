// Source: F:/Games/Dayz/scripts/4_world/entities/undergroundstash.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UndergroundStash extends ItemBase
{
    void UndergroundStash() {}
    
    void PlaceOnGround()
    {
        vector pos = GetPosition();
        pos[1] = g_Game.SurfaceRoadY(pos[0], pos[2]);
        pos[1] = pos[1] + 0.22;
        SetPosition(pos);
