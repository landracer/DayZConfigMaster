// ======================================================================
// File: pawn_8c_source.html
// Category: other
// ======================================================================

#ifdef FEATURE_NETWORK_RECONCILIATION
typedef Serializer PawnStateWriter;
typedef Serializer PawnStateReader;
typedef Serializer PawnMoveWriter;
typedef Serializer PawnMoveReader;
class PawnOwnerState
{
protected void PawnOwnerState() {}
protected void ~PawnOwnerState() {}
proto native void SetMoveId(int value);
proto native int GetMoveId();
proto native int GetSimulationTimestamp();
proto native void SetPhysicsTimeStamp(int value);
proto native int GetPhysicsTimeStamp();
proto native void SetWaterTime(float value);
proto native float GetWaterTime();
protected event void Write(PawnStateWriter ctx)
{
}
protected event void Read(PawnStateReader ctx)
{
}
#ifdef DIAG_DEVELOPER
protected event void DiagWrite(PawnStateWriter ctx)
{
}
protected event void DiagRead(PawnStateReader ctx)
{
}
#endif
protected event int EstimateMaximumSize()
{
return 0;
}
#ifdef DIAG_DEVELOPER
event void GetTransform(inout vector transform[4])
{
}
#endif
};
class PawnMove
{
protected void PawnMove() {}
protected void ~PawnMove() {}
proto native void SetMoveId(int value);
proto native int GetMoveId();
proto native void SetSimulationTimestamp(int value);
proto native int GetSimulationTimestamp();
proto native void SetTimeSlice(float value);
proto native float GetTimeSlice();
protected event void Write(PawnMoveWriter ctx, PawnMove prev)
{
}
protected event void Read(PawnMoveReader ctx, PawnMove prev)
{
}
protected event int EstimateMaximumSize()
{
return 0;
}
#ifdef DIAG_DEVELOPER
event void GetTransform(inout vector transform[4])
{
}
#endif
};
enum NetworkMoveStrategy
{
NONE,
LATEST,
PHYSICS,
};
enum NetworkRewindType
{
NOT_SET,
DROP,
REPLAY,
ADDITIVE
};
enum NetworkCompareResult
{
APPROVE,
CORRECT,
FORCE_CORRECT
};
class Pawn : EntityAI
{
proto native bool IsOwner();
proto native bool IsAuthority();
proto native bool IsAuthorityOwner();
proto native bool IsProxy();
proto native void ForceCorrection();
proto native PlayerIdentity GetOwnerIdentity();
proto native PawnOwnerState GetOwnerState();
proto native PawnMove GetNextMove();
proto native NetworkMoveStrategy GetNetworkMoveStrategy();
protected event void OnPossess()
{
}
protected event void OnUnPossess()
{
}
protected event typename GetOwnerStateType()
{
return PawnOwnerState;
}
protected event typename GetMoveType()
{
return PawnMove;
}
protected event void ObtainMove(/*inout*/ PawnMove pMove)
{
}
protected event NetworkCompareResult CompareMove(PawnMove pMove)
{
return NetworkCompareResult.APPROVE;
}
protected event void ConsumeMove(PawnMove pMove)
{
}
protected event bool ReplayMove(PawnMove pMove)
{
return true;
}
protected event void SimulateMove(PawnMove pMove)
{
}
protected event void ObtainState(/*inout*/ PawnOwnerState pState)
{
}
protected event void RewindState(PawnOwnerState pState, /*inout*/ PawnMove pMove, inout NetworkRewindType pRewindType)
{
}
protected event void ReplayAdditiveMove(/*inout*/ PawnMove pMove, /*const*/ PawnMove pDeltaMove)
{
}
protected event void ApplyAdditiveMove(PawnMove pDeltaMove)
{
}
};
#endif