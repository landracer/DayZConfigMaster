// Source: F:/Games/Dayz/scripts/2_gamelib/entities/scriptmodel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE
 
[EditorAttribute(&quot;box&quot;, &quot;GameLib/Scripted&quot;, &quot;Script model&quot;, &quot;-0.25 -0.25 -0.25&quot;, &quot;0.25 0.25 0.25&quot;, &quot;255 0 0 255&quot;, &quot;0 0 0 0&quot;, true, true, true)]
class ScriptModelClass
{
 
}
 
ScriptModelClass ScriptModelSource;
 
class ScriptModel: GenericEntity
{
    [Attribute(&quot;&quot;, &quot;resourceNamePicker&quot;, &quot;Model&quot;, &quot;xob&quot;)]
    string Model;
    [Attribute(&quot;1&quot;, &quot;combobox&quot;, &quot;Physics&quot;, &quot;&quot;, { ParamEnum(&quot;None&quot;, &quot;2&quot;), ParamEnum(&quot;Static&quot;, &quot;1&quot;), ParamEnum(&quot;Dynamic&quot;, &quot;0&quot;) } )]
    int Type;
 
    void ScriptModel(IEntitySource src, IEntity parent)
    {
        if (Model== &quot;&quot;)
            return;
 
        SetFlags(EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false);
        
        vobject vobj = GetObject(Model);
        SetObject(vobj, &quot;&quot;);
        ReleaseObject(vobj, false);
 
        if (Type == 1)
        {
            dBodyCreateStatic(this, 0xffffffff); // todo - defines for layer mask           
        }
        else if (Type == 0)
        {
            if(!dBodyCreateDynamic(this, 1.0, 0xffffffff))
            {
                //create implicit box
                vector mins, maxs;
                GetBounds(mins, maxs);
                vector center = (mins + maxs) * 0.5;
                vector size = maxs - mins;
                                
                PhysicsGeomDef geoms[] = {PhysicsGeomDef(&quot;&quot;, dGeomCreateBox(size), &quot;material/default&quot;, 0xffffffff)};
                dBodyCreateDynamicEx(this, center, 1, geoms);
            }
            if(dBodyIsSet(this))
            {
                dBodySetMass(this, 1.0);
                dBodyActive(this, ActiveState.ACTIVE);
                dBodyDynamic(this, true);
            }
        }
    }
 
    void ~ScriptModel()
    {
        if(dBodyIsSet(this))
            dBodyDestroy(this);
    }
}
 
#endif
