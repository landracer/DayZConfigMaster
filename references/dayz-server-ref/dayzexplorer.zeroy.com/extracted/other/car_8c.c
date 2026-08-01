// ======================================================================
// File: car_8c_source.html
// Category: other
// ======================================================================

enum CarSoundCtrl
{
// simulation
ENGINE,
RPM,
SPEED,
// miscellaneous
DOORS,
PLAYER
};
enum CarFluid
{
FUEL,
OIL,
BRAKE,
COOLANT,
USER1,
USER2,
USER3,
USER4
};
enum CarGearboxType
{
MANUAL,
AUTOMATIC
}
enum CarGear
{
REVERSE,
NEUTRAL,
FIRST,
SECOND,
THIRD,
FOURTH,
FIFTH,
SIXTH,
SEVENTH,
EIGTH,
NINTH,
TENTH,
ELEVENTH,
TWELFTH,
THIRTEENTH,
FOURTEENTH,
FIFTEENTH,
SIXTEENTH
};
enum CarAutomaticGearboxMode
{
P,
R,
N,
D
};
enum CarWheelWaterState
{
ON_LAND,
IN_WATER,
UNDER_WATER
};
class CarType : TransportType
{
};
class CarOwnerState : TransportOwnerState
{
};
class CarMove : TransportMove
{
};
class Car extends Transport
{
protected override event typename GetOwnerStateType()
{
return CarOwnerState;
}
protected override event typename GetMoveType()
{
return CarMove;
}
proto native float GetSpeedometer();
float GetSpeedometerAbsolute()
{
return Math.AbsFloat(GetSpeedometer());
}
override bool IsAreaAtDoorFree( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
{
vector transform[4];
vector extents;
extents[0] = horizontalExtents;
extents[1] = playerHeight;
extents[2] = horizontalExtents;
float speed = GetSpeedometerAbsolute();
if (speed > 8)
extents[2] = extents[2] * 6;
if (speed > 8)
extents[0] = 2;
return IsAreaAtDoorFree( currentSeat, maxAllowedObjHeight, extents, transform );
}
override Shape DebugFreeAreaAtDoor( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
{
int color = ARGB(20, 0, 255, 0);
vector transform[4];
vector extents;
extents[0] = horizontalExtents;
extents[1] = playerHeight;
extents[2] = horizontalExtents;
float speed = GetSpeedometerAbsolute();
if (speed > 8)
extents[2] = extents[2] * 6;
if (speed > 8)
extents[0] = 2;
if (!IsAreaAtDoorFree( currentSeat, maxAllowedObjHeight, extents, transform ))
{
color = ARGB(20, 255, 0, 0);
}
Shape shape = Debug.DrawBox(-extents * 0.5, extents * 0.5, color);
shape.SetMatrix(transform);
return shape;
}
protected bool DetectFlippedUsingWheels(VehicleFlippedContext ctx, bool disallowSide)
{
if (disallowSide && (vector.Dot(GetDirectionUp(), vector.Up) < 0.7))
{
// return as "flipped", vehicle isn&#39;t pointing enough up to be reasonably certain
return true;
}
int wheelCount = WheelCount();
for (int wheelIdx = 0; wheelIdx < wheelCount; wheelIdx++)
{
if (!WheelHasContact(wheelIdx))
{
// wheel not in contact, then we could be flipped, we assume there exist other predicates
return true;
}
}
// all wheels in contact (or zero registered wheels), then we are in contact
return false;
}
proto native float GetSteering();
proto native void SetSteering(float value, bool unused0 = false);
proto native float GetThrottle();
proto native void SetThrottle(float value);
proto native int GetClutch();
proto native void SetClutch(float value);
proto native float GetBrake();
proto native void SetBrake(float value, float unused0 = 0, bool unused1 = false);
proto native float GetHandbrake();
proto native void SetHandbrake(float value);
proto native void SetBrakesActivateWithoutDriver(bool activate = true);
proto native float EngineGetRPMMin();
proto native float EngineGetRPMIdle();
proto native float EngineGetRPMMax();
proto native float EngineGetRPMRedline();
proto native float EngineGetRPM();
proto native bool EngineIsOn();
proto native void EngineStart();
proto native void EngineStop();
proto native vector GetEnginePos();
proto native void SetEnginePos(vector pos);
proto native int GetCurrentGear();
proto native int GetGear();
proto native int GetNeutralGear();
proto native int GetGearCount();
proto native void ShiftUp();
proto native void ShiftTo(int gear);
proto native void ShiftDown();
proto native CarGearboxType GearboxGetType();
proto native CarAutomaticGearboxMode GearboxGetMode();
proto native bool WheelIsAnyLocked();
proto native float WheelGetAngularVelocity( int wheelIdx );
proto native bool WheelHasContact( int wheelIdx );
proto native vector WheelGetContactPosition( int wheelIdx );
proto native vector WheelGetContactNormal( int wheelIdx );
proto native vector WheelGetDirection( int wheelIdx );
proto native SurfaceInfo WheelGetSurface( int wheelIdx );
proto native CarWheelWaterState WheelGetWaterState( int wheelIdx );
proto native EntityAI WheelGetEntity( int wheelIdx );
proto native bool WheelIsLocked( int wheelIdx );
proto native int WheelCount();
proto native int WheelCountPresent();
proto native float GetFluidCapacity(CarFluid fluid);
proto native float GetFluidFraction(CarFluid fluid);
proto native void Leak(CarFluid fluid, float amount);
proto native void LeakAll(CarFluid fluid);
proto native void Fill(CarFluid fluid, float amount);
bool OnBeforeEngineStart()
{
// engine can start by default
return true;
}
void OnEngineStart() {}
void OnEngineStop() {}
void OnGearChanged(int newGear, int oldGear)
{
}
void OnFluidChanged(CarFluid fluid, float newValue, float oldValue) {}
float OnSound(CarSoundCtrl ctrl, float oldValue)
{
// just use the computed value by the game code
return oldValue;
}
[Obsolete("no replacement")]
proto native void ForcePosition(vector pos);
[Obsolete("no replacement")]
proto native void ForceDirection(vector dir);
[Obsolete("Use methods directly on Car")]
proto native CarController GetController();
[Obsolete("Use Car.IsTurbo")]
proto native float GetThrustTurbo();
[Obsolete("Use Car.IsGentle")]
proto native float GetThrustGentle();
[Obsolete("Use Car.GetThrottle")]
proto native float GetThrust();
[Obsolete("Use Car.SetThrottle/Car.SetTurbo/Car.SetGentle")]
proto native void SetThrust(float in, float gentle = 0, float turbo = 0);
[Obsolete("no replacement")];
proto native void SetClutchState(bool in);
[Obsolete("Use Car.GetGearCount")]
proto native int GetGearsCount();
};
class CarController
{
private void CarController() {}
private void ~CarController() {}
proto float GetSteering();
proto void SetSteering( float in, bool analog = false );
proto float GetThrustTurbo();
proto float GetThrustGentle();
proto float GetThrust();
proto void SetThrust( float in, float gentle = 0, float turbo = 0 );
proto float GetBrake();
proto void SetBrake( float in, float panic = 0 );
proto int GetGear();
proto void ShiftUp();
proto void ShiftTo( CarGear gear );
proto void ShiftDown();
};