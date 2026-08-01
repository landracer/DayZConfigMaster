// Source: F:/Games/Dayz/scripts/4_world/systems/inventory/entityplacementcallback.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EntityPlacementCallback : ObjectSnapCallback
{
#ifdef DIAG_DEVELOPER
#ifndef SERVER
    ref array<Shape> m_Shapes = new array<Shape>();
#endif
#endif
 
    void EntityPlacementCallback()
    {
