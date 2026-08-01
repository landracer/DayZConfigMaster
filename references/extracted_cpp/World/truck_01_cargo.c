// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/truck_01_cargo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Truck_01_Cargo extends Truck_01_Chassis {};
/*
{
    override int GetAnimInstance()
    {
        return VehicleAnimInstances.V3S;
    }
 
    override int GetSeatAnimationType( int posIdx )
    {
        switch( posIdx )
        {
        case 0:
            return DayZPlayerConstants.VEHICLESEAT_DRIVER;
        case 1:
            return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
        }
 
        return 0;
    }
    
    override bool CrewCanGetThrough( int posIdx )
    {
        CarDoor carDoor;
        switch( posIdx )
        {
            case 0:
                Class.CastTo( carDoor, FindAttachmentBySlotName(&quot;V3SDriverDoors&quot;) );
                if ( carDoor )
                {
                    if ( carDoor.GetAnimationPhase(&quot;DoorsSource&quot;) > 0.5 ) return true;
                }
                else
                {
                    return true;
                }
                return false;
            break;
            
            case 1:
                Class.CastTo( carDoor, FindAttachmentBySlotName(&quot;V3SCoDriverDoors&quot;) );
                if ( carDoor )
                {
                    if ( carDoor.GetAnimationPhase(&quot;DoorsSource&quot;) > 0.5 ) return true;
                }
                else
                {
                    return true;
                }
                return false;
            break;
        }
 
        return false;
    }
 
    override float OnSound( CarSoundCtrl ctrl, float oldValue )
    {
        switch ( ctrl )
        {
            case CarSoundCtrl.DOORS:
                float newValue = 0;
 
                //-----
                CarDoor carDoor;
                Class.CastTo( carDoor, FindAttachmentBySlotName(&quot;V3SDriverDoors&quot;) );
                if ( carDoor )
                {   
                    if ( carDoor.GetAnimationPhase(&quot;DoorsSource&quot;) > 0.5)
                    {
                        newValue = newValue + 0.8;
                    }
                }
                else
                {
                    newValue = newValue + 0.8;
                }
 
                //-----
                Class.CastTo( carDoor, FindAttachmentBySlotName(&quot;V3SCoDriverDoors&quot;) );
                if ( carDoor )
                {   
                    if ( carDoor.GetAnimationPhase(&quot;DoorsSource&quot;) > 0.5)
                    {
                        newValue = newValue + 0.8;
                    }
                }
                else
                {
                    newValue = newValue + 0.8;
                }
            
                if ( newValue > 1 )
                    newValue = 1;
 
                return newValue;
            break;
        }
 
        return oldValue;
    }
 
    override string GetAnimSourceFromSelection( string selection )
    {
        switch( selection )
        {
        case &quot;wheelsideplate1&quot;:
            return &quot;WheelSidePlate1&quot;;
        case &quot;wheelsideplate2&quot;:
            return &quot;WheelSidePlate2&quot;;
        }
 
        return &quot;&quot;;
    }
 
    override bool IsVitalCarBattery()
    {
        return false;
    }
 
    override bool IsVitalSparkPlug()
    {
        return false;
    }
 
    override bool IsVitalGlowPlug()
    {
        return false;
    }
 
    override bool IsVitalEngineBelt()
    {
        return false;
    }
}
*/
