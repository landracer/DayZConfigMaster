// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/iconscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class IconsContainer: Container
{
    protected ref map<int, ref Icon> m_EntitiesMap = new map<int, ref Icon>;
 
    void IconsContainer( LayoutHolder parent )
    {
