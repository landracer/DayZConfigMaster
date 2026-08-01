// Source: F:/Games/Dayz/scripts/2_gamelib/entities/gamelibentities.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//Generic entities from GameLib (script side of c++ classes)
 
#ifdef COMPONENT_SYSTEM
class GenericEntity extends IEntity
{
    //native method implemented on c++ side
    proto native void Show(bool show);
    
    proto native GenericComponent FindComponent(typename typeName);
    
    proto native void InsertComponent(GenericComponent component);
    
    proto native void RemoveComponent(GenericComponent component);
    
    proto native void DeleteComponent(GenericComponent component);
 
#ifdef WORKBENCH
    void _WB_AfterWorldUpdate(float timeSlice) {};
#endif
}
 
class GenericWorldEntity extends GenericEntity
{
}
 
class GenericTerrainEntity extends GenericEntity
{
}
 
class LightEntity extends GenericEntity
{
    proto native external void SetDiffuseColor(int color);
    proto native external int GetDiffuseColor();
    
    proto native external void SetRadius(float radius);
    proto native external float GetRadius();
    
    proto native external void SetConeAngle(float angle);
    proto native external float GetConeAngle();
    
    proto native external void SetCastShadow(bool enable);
    proto native external bool IsCastShadow(bool enable);
}
 
class GenericWorldLightEntity extends GenericEntity
{
}
 
class GenericWorldFogEntity extends GenericEntity
{
}
 
class BasicEntity extends GenericEntity
{
}
 
class WorldEntityClass
{
}
 
class WorldEntity extends GenericWorldEntity
{
}
 
class ModelEntity extends BasicEntity
{
}
 
enum CharacterMovement
{
    MOVEMENTTYPE_IDLE,
    MOVEMENTTYPE_WALK,
    MOVEMENTTYPE_RUN,
    MOVEMENTTYPE_SPRINT
};
 
enum CharacterStance
{
    STANCE_ERECT,
    STANCE_CROUCH,
    STANCE_PRONE,
    STANCE_ERECT_RAISED,
    STANCE_CROUCH_RAISED,
    STANCE_PRONE_RAISED
};
 
class CharacterEntity extends BasicEntity
{
    proto native void Teleport(vector transform[4]);
    proto native CharacterMovement GetCurrentMovement();
    proto native CharacterStance GetCurrentStance();
}
 
class BasicCamera extends BasicEntity
{
}
 
class VRHandEntity extends GenericEntity
{
}
 
#endif
