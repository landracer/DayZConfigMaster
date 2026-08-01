// ======================================================================
// File: component_8c_source.html
// Category: other
// ======================================================================

const string COMP_NAME_NONE = "None";
const int COMP_TYPE_UNDEFINED = -1;
const int COMP_TYPE_ETITY_DEBUG = 0;
const int COMP_TYPE_ENERGY_MANAGER = 1;
const int COMP_TYPE_BODY_STAGING = 2;
const int COMP_TYPE_ANIMAL_BLEEDING = 3;
const int COMP_TYPE_COUNT = 4;
class Component
{
//==========================================
// Variables Private Static
private static string m_CompNames[COMP_TYPE_COUNT];
//==========================================
// Variables Private
protected EntityAI m_ThisEntityAI;
//==========================================
void Event_OnFrame(IEntity other, float timeSlice);
Shape DebugBBoxDraw();
void DebugBBoxSetColor(int color);
void DebugBBoxDelete();
Shape DebugDirectionDraw(float distance = 1);
void DebugDirectionSetColor(int color);
void DebugDirectionDelete();
// Methods Public Static
static void Init()
{
m_CompNames[COMP_TYPE_ETITY_DEBUG] = "ComponentEntityDebug";
m_CompNames[COMP_TYPE_ENERGY_MANAGER] = "ComponentEnergyManager";
m_CompNames[COMP_TYPE_BODY_STAGING] = "ComponentBodyStaging";
m_CompNames[COMP_TYPE_ANIMAL_BLEEDING] = "ComponentAnimalBleeding";
}
// GetNameByType
static string GetNameByType(int comp_type)
{
if ( IsTypeExist(comp_type) == false )
{
LogErrorBadCompType(comp_type, "Component->GetNameByType()");
return "None";
}
return m_CompNames[comp_type];
}
// IsTypeExist
static bool IsTypeExist(int comp_type)
{
if ( comp_type < 0 || comp_type >= COMP_TYPE_COUNT )
{
return false;
}
return true;
}
// LogErrorBadCompType
static void LogErrorBadCompType(int comp_type, string fnc_name)
{
string msg = "Bad parameter comp_type=&#39;"+comp_type.ToString()+"&#39;. Parameter must be 0-"+(COMP_TYPE_COUNT - 1).ToString()+". Returning component name &#39;None&#39;.";
Debug.LogError(msg, "Component", "n/a", fnc_name);
}
// LogWarningAlredyExist
static void LogWarningAlredyExist(int comp_type, string fnc_name)
{
string msg = "Component &#39;"+Component.GetNameByType(comp_type)+"&#39; already exists!";
Debug.LogError(msg, "Component", "n/a", fnc_name);
}
//=======================================public
// SetParentEntityAI
//=============================================
void SetParentEntityAI(EntityAI e)
{
m_ThisEntityAI = e;
}
//====================================protected
// Awake
//=============================================
void Event_OnAwake()
{
}
//====================================protected
// Init
//=============================================
void Event_OnInit()
{
}
//=======================================public
// Log
//=============================================
void LogThis(string msg, string fnc_name = "n/a")
{
//Debug.Log(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.ToString());
}
//=======================================public
// LogWarning
//=============================================
void LogThisWarning(string msg, string fnc_name = "n/a")
{
Debug.LogWarning(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.ToString());
}
//=======================================public
// LogError
//=============================================
void LogThisError(string msg, string fnc_name = "n/a")
{
Debug.LogError(msg, GetCompName(), "n/a", fnc_name, m_ThisEntityAI.ToString());
}
//=======================================public
// GetType
//=============================================
string GetCompName()
{
return Component.GetNameByType(this.GetCompType());
}
//=======================================public
// GetType
//=============================================
int GetCompType()
{
return COMP_TYPE_UNDEFINED;
}
//=======================================public
// Event_OnItemAttached
//=============================================
void Event_OnItemAttached(EntityAI item, string slot_name)
{
LogThis("" + item + " -> " + slot_name,"Event_OnItemAttached");
//Debug.Log("Component=>Event_OnItemAttached: " + item + " -> " + slot_name, );
}
//=======================================public
// Event_OnItemDetached
//=============================================
void Event_OnItemDetached(EntityAI item, string slot_name)
{
LogThis("" + item + " <- " + slot_name,"Event_OnItemDetached");
//Log("Component=>Event_OnItemDetached: " + item + " <- " + slot_name );
}
}