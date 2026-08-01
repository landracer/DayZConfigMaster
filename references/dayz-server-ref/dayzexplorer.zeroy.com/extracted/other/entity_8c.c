// ======================================================================
// File: entity_8c_source.html
// Category: other
// ======================================================================

class Entity extends ObjectTyped
{
proto native void DisableSimulation(bool disable);
proto native bool GetIsSimulationDisabled();
proto native int GetSimulationTimeStamp();
proto native float GetAnimationPhase(string animation);
proto native void SetAnimationPhase(string animation, float phase);
proto int GetNumUserAnimationSourceNames();
proto string GetUserAnimationSourceName(int index);
void SetAnimationPhaseNow(string animation, float phase)
{
ResetAnimationPhase(animation, phase);
SetAnimationPhase(animation, phase);
}
proto native void ResetAnimationPhase(string animation, float phase);
void OnAnimationPhaseStarted(string animSource, float phase);
proto native int GetBoneIndex( string proxySelectionName );
proto native Object GetBoneObject( int boneIndex );
proto native void SetInvisible(bool invisible);
void OnInvisibleSet(bool invisible);
proto void MoveInTime(vector targetTransform[4], float deltaT);
void OnCreatePhysics();
bool OnNetworkTransformUpdate(out vector pos, out vector ypr);
};