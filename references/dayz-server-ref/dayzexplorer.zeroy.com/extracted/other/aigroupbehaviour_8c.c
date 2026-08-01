// ======================================================================
// File: aigroupbehaviour_8c_source.html
// Category: other
// ======================================================================

class AIGroupBehaviour : Managed
{
private void AIGroupBehaviour();
private void ~AIGroupBehaviour();
}
class BehaviourGroupInfectedPackWaypointParams
{
vector m_CenterPosition;
float m_Radius;
void BehaviourGroupInfectedPackWaypointParams(vector pos, float radius)
{
m_CenterPosition = pos;
m_Radius = radius;
}
}
class BehaviourGroupInfectedPack : AIGroupBehaviour
{
proto native void SetWaypoints(array<ref BehaviourGroupInfectedPackWaypointParams> waypointParams, int waypointDefaultIndex, bool forwardDirection, bool loop);
proto native void SetWaypointsTraverseDirection(bool forwardDirection);
proto native void SetWaypointsLoop(bool loop);
proto native void SetCurrentWaypoint(int waypointIndex);
}