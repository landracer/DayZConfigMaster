// ======================================================================
// File: 3__game_2entities_2building_8c_source.html
// Category: entities
// ======================================================================

typedef Param1<int> DoorStartParams;
typedef Param2<int, bool> DoorFinishParams;
typedef Param1<int> DoorLockParams;
class BuildingType : EntityAIType
{
};
class Building : EntityAI
{
proto native int GetLaddersCount();
proto native vector GetLadderPosTop(int ladderIndex);
proto native vector GetLadderPosBottom(int ladderIndex);
proto native int GetDoorIndex(int componentIndex);
proto native int GetDoorCount();
proto native bool IsDoorOpen(int index);
proto native bool IsDoorOpening(int index);
proto native bool IsDoorOpeningAjar(int index);
proto native bool IsDoorClosing(int index);
proto native bool IsDoorOpened(int index);
proto native bool IsDoorOpenedAjar(int index);
proto native bool IsDoorClosed(int index);
proto native bool IsDoorLocked(int index);
proto native void PlayDoorSound(int index);
proto native void OpenDoor(int index);
proto native void CloseDoor(int index);
proto native void LockDoor(int index, bool force = false);
proto native void UnlockDoor(int index, bool animate = true);
proto native vector GetDoorSoundPos(int index);
proto native float GetDoorSoundDistance(int index);
proto native void OutputDoorLog();
int GetNearestDoorBySoundPos(vector position)
{
float smallestDist = float.MAX;
int nearestDoor = -1;
int count = GetDoorCount();
for (int i = 0; i < count; i++)
{
float dist = vector.DistanceSq(GetDoorSoundPos(i), position);
if (dist < smallestDist)
{
nearestDoor = i;
smallestDist = dist;
}
}
return nearestDoor;
}
void OnDoorOpenStart(DoorStartParams params)
{
}
void OnDoorOpenFinish(DoorFinishParams params)
{
}
void OnDoorOpenAjarStart(DoorStartParams params)
{
}
void OnDoorOpenAjarFinish(DoorFinishParams params)
{
}
void OnDoorCloseStart(DoorStartParams params)
{
}
void OnDoorCloseFinish(DoorFinishParams params)
{
}
void OnDoorLocked(DoorLockParams params)
{
}
void OnDoorUnlocked(DoorLockParams params)
{
}
bool CanDoorBeOpened(int doorIndex, bool checkIfLocked = false)
{
if (IsDoorOpen(doorIndex))
return false;
if (checkIfLocked)
{
if (IsDoorLocked(doorIndex))
return false;
}
else
{
if (!IsDoorLocked(doorIndex))
return false;
}
return true;
}
bool CanDoorBeClosed(int doorIndex)
{
return IsDoorOpen(doorIndex);
}
bool CanDoorBeLocked(int doorIndex)
{
return (!IsDoorOpen(doorIndex) && !IsDoorLocked(doorIndex));
}
int GetLockCompatibilityType(int doorIdx)
{
return 1 << EBuildingLockType.LOCKPICK; //all doors are lockpickable by default
}
override void GetDebugActions(out TSelectableActionInfoArrayEx outputList)
{
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_OUTPUT_LOG, "Output Door Log", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_PLAY_DOOR_SOUND, "Play Door Sound", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_OPEN_DOOR, "Open Door", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_CLOSE_DOOR, "Close Door", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_LOCK_DOOR, "Lock Door", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_UNLOCK_DOOR, "Unlock Door", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
if (Gizmo_IsSupported())
{
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.GIZMO_OBJECT, "Gizmo Object", FadeColors.LIGHT_GREY));
outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
}
super.GetDebugActions(outputList);
}
override bool OnAction(int action_id, Man player, ParamsReadContext ctx)
{
if (super.OnAction(action_id, player, ctx))
return true;
if (g_Game.IsClient() || !g_Game.IsMultiplayer())
{
switch (action_id)
{
case EActions.GIZMO_OBJECT:
if (GetGizmoApi())
GetGizmoApi().SelectObject(this);
return true;
}
}
switch (action_id)
{
case EActions.BUILDING_PLAY_DOOR_SOUND:
PlayDoorSound(GetNearestDoorBySoundPos(player.GetPosition()));
return true;
}
if (!g_Game.IsServer())
return false;
switch (action_id)
{
case EActions.BUILDING_OUTPUT_LOG:
OutputDoorLog();
return true;
case EActions.BUILDING_OPEN_DOOR:
OpenDoor(GetNearestDoorBySoundPos(player.GetPosition()));
return true;
case EActions.BUILDING_CLOSE_DOOR:
CloseDoor(GetNearestDoorBySoundPos(player.GetPosition()));
return true;
case EActions.BUILDING_LOCK_DOOR:
LockDoor(GetNearestDoorBySoundPos(player.GetPosition()));
return true;
case EActions.BUILDING_UNLOCK_DOOR:
UnlockDoor(GetNearestDoorBySoundPos(player.GetPosition()));
return true;
}
return false;
}
override bool IsBuilding()
{
return true;
}
override bool CanObstruct()
{
return true;
}
override bool IsHealthVisible()
{
return false;
}
ref TIntArray m_InteractActions;
void Building()
{
m_InteractActions = new TIntArray;
g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " InteractActions", m_InteractActions);
}
override bool IsInventoryVisible()
{
return false;
}
override int GetMeleeTargetType()
{
return EMeleeTargetType.NONALIGNABLE;
}
};
//-----------------------------------------------------------------------------
class WindSockType : EntityType
{
};
//-----------------------------------------------------------------------------
/*
class WindSock : Entity
{
};
*/