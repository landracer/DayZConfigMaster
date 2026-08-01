// ======================================================================
// File: animphysagent_8c_source.html
// Category: other
// ======================================================================

#ifdef FEATURE_NETWORK_RECONCILIATION
enum AnimPhysCorrectionType
{
MINIMAL,
DROP,
FULL
};
class AnimPhysOwnerState : PawnOwnerState
{
proto native void SetCorrectionType(int value);
proto native int GetCorrectionType();
proto native void SetPosition(vector value);
proto native void GetPosition(out vector value);
proto native void SetRotation(float value[4]);
proto native void GetRotation(out float value[4]);
proto native void SetParent(Object value);
proto native Object GetParent();
proto native void SetLinked(Object value);
proto native Object GetLinked();
proto native bool IsLinked();
proto native bool IsParented();
proto native void SetSpeed(vector value);
proto native void GetSpeed(out vector value);
proto native void SetCollisionLayer(int value);
proto native int GetCollisionLayer();
proto native void SetFlags(int value);
proto native int GetFlags();
proto native void SetCollisionOffset(vector value);
proto native int GetCollisionOffset(out vector value);
proto native void SetStance(int value);
proto native int GetStance();
#ifdef DIAG_DEVELOPER
override event void GetTransform(inout vector transform[4])
{
float rotation[4];
GetRotation(rotation);
Math3D.QuatToMatrix(rotation, transform);
vector position;
GetPosition(position);
transform[3] = position;
Object parent = GetParent();
if (parent)
{
vector parentTransform[4];
parent.GetTransform(parentTransform);
Math3D.MatrixMultiply4(parentTransform, transform, transform);
}
}
#endif
};
class AnimPhysMove : PawnMove
{
proto native void SetCorrectionType(int value);
proto native int GetCorrectionType();
proto native void SetPosition(vector value);
proto native void GetPosition(out vector value);
proto native void SetRotation(float value[4]);
proto native void GetRotation(out float value[4]);
proto native void SetParent(Object value);
proto native void SetLinked(Object value);
proto native Object GetParentOrLinked();
proto native Object GetParent();
proto native bool HasParent();
proto native bool IsParented();
proto native bool IsLinked();
proto native void SetParentTransform(vector transform[4]);
proto native void GetParentTransform(out vector transform[4]);
#ifdef DIAG_DEVELOPER
override event void GetTransform(inout vector transform[4])
{
float rotation[4];
GetRotation(rotation);
Math3D.QuatToMatrix(rotation, transform);
vector position;
GetPosition(position);
transform[3] = position;
if (HasParent())
{
vector parentTransform[4];
GetParentTransform(parentTransform);
Math3D.MatrixMultiply4(parentTransform, transform, transform);
}
}
#endif
};
#endif