// Source: F:/Games/Dayz/scripts/1_core/proto/enworld.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
//----------------------------------------------
 
typedef int[] WorldHandle;
 
proto native float GetWorldTime();

proto native WorldHandle SetCurrentWorld(WorldHandle world);
 
//proto native void SchedulePreload(vector pos, float radius);
 
proto native IEntity FindEntityByName(IEntity worldEnt, string name);
proto native IEntity FindEntityByID(IEntity worldEnt, int ID);

proto native int GetNumActiveEntities(IEntity worldEntity);
proto native IEntity GetActiveEntity(IEntity worldEntity, int index);
 
//----------------------------------------------
 
enum CameraType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
