// Source: F:/Games/Dayz/scripts/2_gamelib/components/gamelibcomponents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef COMPONENT_SYSTEM
//Generic components from GameLib (script side of c++ classes)
 
typedef int[] IEntityComponentSource;
class IEntityComponentSource: BaseContainer
{
};

enum TouchEvent
{
    ON_ENTER,
    ON_STAY,
    ON_EXIT
};

//TypeID MeshObjectTypeID;
//TypeID HierarchyTypeID;
//TypeID RigidBodyTypeID;
//TypeID SphereGeometryTypeID;
//TypeID BoxGeometryTypeID;
 
 
class GenericComponent : Managed
{
    proto native int GetEventMask();
    
    proto native int SetEventMask(IEntity owner, int mask);
    
    proto native int ClearEventMask(IEntity owner, int mask);

    proto native void Activate(IEntity owner);

    proto native void Deactivate(IEntity owner);

    proto native bool IsActive();
    
    protected void GenericComponent(IEntityComponentSource src, IEntity ent);   
}
 
class GenericComponentClass
{
    bool DependsOn(typename otherClass, TypeID otherTypeID) {}
}

class ScriptComponent : GenericComponent
{
    /*
    Event when owner entity is touched
    \param owner
    Touched entity
    \param extra
    Bitmask of touch types TODO
    */
    protected void EOnTouch(IEntity owner, int extra);

    protected void EOnInit(IEntity owner, int extra);

    protected void EOnExtra(IEntity owner, int extra);

    protected void EOnNotVisible(IEntity owner, int extra);

    protected void EOnVisible(IEntity owner, int extra);

    protected void EOnFrame(IEntity owner, float timeSlice);

    protected void EOnPostFrame(IEntity owner, int extra);

    protected void EOnAnimEvent(IEntity owner, AnimEvent extra);

    protected void EOnSoundEvent(IEntity owner, SoundEvent extra);

    protected void EOnPostSimulate(IEntity owner, float timeslice);

    protected void EOnSimulate(IEntity owner, float timeslice);

    protected void EOnJointBreak(IEntity owner, int extra);
    protected void EOnPhysicsMove(IEntity owner, int extra);

    protected void EOnContact(IEntity owner, Contact extra);

    protected void EOnActivate(IEntity owner);

    protected void EOnDeactivate(IEntity owner);

    protected void OnComponentInsert(IEntity owner, ScriptComponent other);

    protected void OnComponentRemove(IEntity owner, ScriptComponent other);

    protected void OnDelete(IEntity owner);
}
 
typedef int[] SoundHandle;
 
class SignalInput
{
    string m_name;
    float m_value;
    
    void SignalInput()
    {
        m_value = 0;
    }
};  
 
class BaseSoundComponent : GenericComponent
{
    /* Get list of &#39;events&#39;. */
    proto native int GetEventNames(out array<string> events);
    /* Get list of &#39;signals. &#39;*/
    proto native int GetSignalNames(out array<string> signals);
    /* Convert signal name to index. */
    proto native int GetSignalIndex(string name);
    /* Set signal value by &#39;name&#39;. */
    proto native void SetSignalValueName(string signal, float value);
    /* Set signal value by &#39;index&#39;. */
    proto native void SetSignalValue(int index, float value);
    /* Play &#39;event&#39;. */
    proto native SoundHandle Play(string name);
    /* Play sounds based on triggers. */
    proto native SoundHandle Update();
    /* Terminate &#39;sound&#39;. */
    proto native void Terminate(SoundHandle handle);
    /* Check if &#39;sound&#39; is played. */
    proto native bool IsPlayed(SoundHandle handle);
    /* Validate handle. */
    proto native bool IsHandleValid(SoundHandle handle);
    /* Set sound position. */
    proto native void SetTransform(vector[] transf);
    /* Enable debug mode. */
    proto native void SetDebug(bool value);
};
 
#endif
