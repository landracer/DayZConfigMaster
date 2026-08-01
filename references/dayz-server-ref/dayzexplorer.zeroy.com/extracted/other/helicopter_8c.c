// ======================================================================
// File: helicopter_8c_source.html
// Category: other
// ======================================================================

class HelicopterType : TransportType
{
};
class Helicopter extends Transport
{
};
class HelicopterAuto extends Helicopter
{
proto native bool IsEngineOn();
proto native void EngineStart();
proto native void EngineStop();
proto native bool IsAutohoverOn();
proto native void AutohoverOn();
proto native void AutohoverOff();
};