// ======================================================================
// File: boat_8c_source.html
// Category: other
// ======================================================================

enum BoatSoundCtrl
{
// simulation
ENGINE,
SPEED,
// miscellaneous
PLAYER
};
enum BoatFluid
{
FUEL
};
class BoatType : TransportType
{
};
class BoatOwnerState : TransportOwnerState
{
};
class BoatMove : TransportMove
{
};
class Boat extends Transport
{
protected override event typename GetOwnerStateType()
{
return BoatOwnerState;
}
protected override event typename GetMoveType()
{
return BoatMove;
}
proto native float GetSteering();
proto native void SetSteering(float value);
proto native float GetThrottle();
proto native void SetThrottle(float value);
proto native int GetClutch();
proto native void SetClutch(float value);
proto native bool HasEngine();
proto native float EngineGetRPMMin();
proto native float EngineGetRPMIdle();
proto native float EngineGetRPMMax();
proto native float EngineGetRPMRedline();
proto native float EngineGetRPM();
proto native bool EngineIsOn();
proto native void EngineStart();
proto native void EngineStop();
proto native int GetCurrentGear();
proto native int GetGear();
proto native int GetNeutralGear();
proto native int GetGearCount();
proto native void ShiftUp();
proto native void ShiftTo(int gear);
proto native void ShiftDown();
proto native vector PropellerGetPosition();
proto native float PropellerGetAngularVelocity();
proto native float GetFluidCapacity(BoatFluid fluid);
proto native float GetFluidFraction(BoatFluid fluid);
proto native void Leak(BoatFluid fluid, float amount);
proto native void LeakAll(BoatFluid fluid);
proto native void Fill(BoatFluid fluid, float amount);
bool OnBeforeEngineStart()
{
// engine can start by default
return true;
}
void OnEngineStart() {}
void OnEngineStop() {}
void OnGearChanged(int newGear, int oldGear) {}
void OnFluidChanged(BoatFluid fluid, float newValue, float oldValue) {}
float OnSound(BoatSoundCtrl ctrl, float oldValue)
{
// just use the computed value by the game code
return oldValue;
}
};