// ======================================================================
// File: enentity_8c_source.html
// Category: other
// ======================================================================

typedef int[] BaseContainer;
typedef int[] IEntitySource;
typedef int[] WidgetSource;
class BaseContainer
{
proto native owned string GetClassName();
proto native owned string GetName();
proto native int VarIndex(string varName);
proto native bool IsVariableSet(int varIndex);
proto bool IsType(int varIndex, typename type);
proto bool Get(int varIndex, out void val);
};
class IEntitySource: BaseContainer
{
proto native IEntitySource GetChildren();
proto native IEntitySource GetSibling();
proto native IEntitySource GetParent();
};
class WidgetSource: BaseContainer
{
proto native WidgetSource GetChildren();
proto native WidgetSource GetSibling();
proto native WidgetSource GetParent();
};
enum EntityEvent
{
TOUCH,
VISIBLE,
NOTVISIBLE,
FRAME,
POSTFRAME,
INIT,
//Only with MeshObject
//ANIMEND - End of AF_ONCE animation, extra = slot number 0...11
//ANIMBLEND - Animation blended from previous one, extra = slot number 0...11
//SOUNDEND - playing of SFL_ONCE sound has ended
// extra = pointer to a sound handle
//USER
JOINTBREAK,
SIMULATE,
POSTSIMULATE,
PHYSICSMOVE,
CONTACT,
EXTRA,
ANIMEVENT,
SOUNDEVENT,
PHYSICSSTEADY,
USER,
ENTER,
LEAVE,
ALL
};
enum EntityFlags
{
VISIBLE,
SOLID,
TRIGGER,
TOUCHTRIGGERS,
SYNCHRONIZATION_DIRTY,
FEATURE,
TRANSLUCENT,
WATER,
ACTIVE,
STATIC,
USER1,
USER2,
USER3,
USER4,
USER5,
USER6
};
class IEntity: Managed
{
//DO NOT INSERT ANYTHING BELOW - order of event methods matters!
event protected void EOnTouch(IEntity other, int extra)
{
}
event protected void EOnInit(IEntity other, int extra)
{
}
event protected void EOnExtra(IEntity other, int extra)
{
}
event protected void EOnNotVisible(IEntity other, int extra)
{
}
event protected void EOnFrame(IEntity other, float timeSlice)
{
}
event protected int EOnVisible(IEntity other, int extra)
{
}
event protected void EOnPostFrame(IEntity other, int extra)
{
}
event protected void EOnWorldProcess(IEntity other, int extra)
{
}
event protected void EOnAnimEvent(IEntity other, AnimEvent extra)
{
}
event protected void EOnSoundEvent(IEntity other, SoundEvent extra)
{
}
event protected void EOnSimulate(IEntity other, float dt)
{
}
event protected void EOnPostSimulate(IEntity other, float timeSlice)
{
}
event protected void EOnJointBreak(IEntity other, int extra)
{
}
event protected void EOnPhysicsMove(IEntity other, int extra)
{
}
event protected void EOnContact(IEntity other, Contact extra)
{
}
protected void EOnUser0(IEntity other, int extra)
{
}
protected void EOnUser1(IEntity other, int extra)
{
}
event protected void EOnEnter(IEntity other, int extra)
{
}
event protected void EOnLeave(IEntity other, int extra)
{
}
protected void EOnUser4(IEntity other, int extra)
{
}
protected void EOnDummy020(IEntity other, int extra)
{
}
protected void EOnDummy021(IEntity other, int extra)
{
}
protected void EOnDummy022(IEntity other, int extra)
{
}
protected void EOnDummy023(IEntity other, int extra)
{
}
protected void EOnDummy024(IEntity other, int extra)
{
}
protected void EOnDummy025(IEntity other, int extra)
{
}
protected void EOnDummy026(IEntity other, int extra)
{
}
protected void EOnDummy027(IEntity other, int extra)
{
}
protected void EOnDummy028(IEntity other, int extra)
{
}
protected void EOnDummy029(IEntity other, int extra)
{
}
protected void EOnDummy030(IEntity other, int extra)
{
}
protected void EOnDummy031(IEntity other, int extra)
{
}
//DO NOT INSERT ANYTHING ABOVE - order of event methods matters!
proto external void GetTransform(out vector mat[]);
proto external void GetRenderTransform(out vector mat[]);
proto external void GetLocalTransform(out vector mat[]);
proto native external vector GetTransformAxis(int axis);
proto native external void SetTransform(vector mat[4]);
proto native external vector GetOrigin();
proto external vector GetLocalPosition();
proto native external vector GetYawPitchRoll();
proto native external vector GetAngles();
proto native external vector GetLocalYawPitchRoll();
proto native external vector GetLocalAngles();
proto native external void SetYawPitchRoll(vector angles);
proto native external void SetAngles(vector angles);
proto native external void SetOrigin(vector orig);
proto native external float GetScale();
proto native external void SetScale(float scale);
proto native external vector VectorToParent(vector vec);
proto native external vector CoordToParent(vector coord);
proto native external vector VectorToLocal(vector vec);
proto native external vector CoordToLocal(vector coord);
proto native int GetID();
proto native void SetID(int id);
proto native void SetName(string name);
proto native external owned string GetName();
proto native external bool AddChild(notnull IEntity child, int pivot, bool positionOnly = false);
proto native external bool RemoveChild(notnull IEntity child, bool keepTransform = false);
proto native external Physics GetPhysics();
proto native bool IsHierarchyPositionOnly();
proto native int GetHierarchyPivot();
proto native IEntity GetParent();
proto native IEntity GetChildren();
proto native IEntity GetSibling();
proto external void GetBounds(out vector mins, out vector maxs);
proto external void GetWorldBounds(out vector mins, out vector maxs);
proto native external EntityFlags GetFlags();
proto native external bool IsFlagSet(EntityFlags flags);
proto native external EntityFlags SetFlags(EntityFlags flags, bool recursively);
proto native external EntityFlags ClearFlags(EntityFlags flags, bool recursively);
proto native external EntityEvent GetEventMask();
proto native external EntityEvent SetEventMask(EntityEvent e );
proto native external EntityEvent ClearEventMask(EntityEvent e);
proto external volatile void SendEvent(notnull IEntity actor, EntityEvent e, void extra);
proto native external void SetObject(vobject object, string options);
proto native vobject GetVObject();
proto native external int Animate(float speed, int loop);
proto native external int AnimateEx(float speed, int loop, out vector lin, out vector ang);
proto native external int SetCameraMask(int mask);
proto native external void FilterNextTrace();
proto native external int Update();
#ifdef COMPONENT_SYSTEM
protected void IEntity(IEntitySource src, IEntity parent);
#endif
};
#ifdef ENF_DONE
// Set fixed LOD. -1 for non-fixed LOD
proto native void SetFixedLOD(IEntity ent, int lod);
//Sets the texture that can be referenced from material as $renderview
//and connects it with camera cam_index. Size iz recommended size of
//rendertarget (0 is default)
proto native void SetRenderView(IEntity ent, int cam_index, int width, int height);
proto void GetRenderView(IEntity ent, out int cam_index, out int width, out int height);
#endif
class ParamEnum: Managed
{
string m_Key;
string m_Value;
string m_Desc;
void ParamEnum(string key, string value, string desc = "")
{
m_Key = key;
m_Value = value;
m_Desc = desc;
}
}
class ParamEnumArray: array<ref ParamEnum>
{
static ParamEnumArray FromEnum(typename e)
{
ParamEnumArray params = new ParamEnumArray();
int cnt = e.GetVariableCount();
int val;
for (int i = 0; i < cnt; i++)
{
if (e.GetVariableType(i) == int && e.GetVariableValue(NULL, i, val))
{
params.Insert(new ParamEnum(e.GetVariableName(i), val.ToString()));
}
}
return params;
}
}
// -------------------------------------------------------------------------
class Attribute
{
string m_DefValue;
string m_UiWidget;
string m_RangeScale;
string m_Desc;
ref ParamEnumArray m_Enums;
void Attribute(string defvalue, string uiwidget, string desc = "", string rangescale = "", ParamEnumArray enums = NULL)
{
m_DefValue = defvalue;
m_UiWidget = uiwidget;
m_RangeScale = rangescale;
m_Desc = desc;
m_Enums = enums;
}
}
class EditorAttribute
{
string m_Style;
string m_Category;
string m_Description;
vector m_SizeMin;
vector m_SizeMax;
string m_Color;
string m_Color2;
bool m_Visible;
bool m_Insertable;
bool m_DynamicBox;
void EditorAttribute(string style, string category, string description, vector sizeMin, vector sizeMax, string color, string color2 = "0 0 0 0", bool visible = true, bool insertable = true, bool dynamicBox = false)
{
m_Style = style;
m_Category = category;
m_Description = description;
m_SizeMin = sizeMin;
m_SizeMax = sizeMax;
m_Color = color;
m_Color2 = color2;
m_Visible = visible;
m_Insertable = insertable;
m_DynamicBox = dynamicBox;
}
}