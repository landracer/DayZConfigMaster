// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/camping/camping.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CamoNetShelter : ItemBase {};
//cluttercutter classes
class MediumTentClutterCutter: Static {};
class LargeTentClutterCutter: Static {};
class SeaChest: DeployableContainer_Base
{
    void SeaChest()
    {
        m_HalfExtents = Vector(0.2,0.5,0.4);
