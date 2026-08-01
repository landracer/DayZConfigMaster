// Source: F:/Games/Dayz/scripts/4_world/entities/gardenbase/gardenplot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GardenPlot extends GardenBase
{
    Object  m_ClutterCutter;
    private const int GARDEN_SLOT_COUNT = 9;
    private const float PLACEMENT_HEIGHT_LIMIT = 0.3; // Y coord placement limit - this is important when server has collision checks disabled
    
    void GardenPlot()
    {
        SetBaseFertility(0.5);
