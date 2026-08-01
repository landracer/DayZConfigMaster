// ======================================================================
// File: headtorch__grey_8c_source.html
// Category: other
// ======================================================================

class Headtorch_Grey : Headtorch_ColorBase
{
override void OnDebugSpawn()
{
GetInventory().CreateInInventory("Battery9V");
}
}