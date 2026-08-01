// Source: F:/Games/Dayz/scripts/4_world/classes/sceneeditor/sceneobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SceneObject
{
    static const int    COLOR_OBJ_BBOX_NORMAL   = 0x00000000;
    static const int    COLOR_OBJ_BBOX_SELECT   = 0x1f007C00;
    
    protected EntityAI                      m_ObjectPtr;
    protected Shape                             m_DebugShapeBBox;
    protected string                            m_InitScript;
    protected string                            m_ObjectName;
    protected string                            m_ObjectNameOrigin;
    
    protected ref array<SceneObject>        m_LinkedSceneObjects;
    protected ref map<SceneObject, Shape>   m_LinkedSceneObjectsShapes;
    
    ref array<int>                          m_LinkedSceneObjectsIndices;
    
    //========================================
    // SceneObject
    //========================================
    SceneObject Init(string obj_name, vector pos)
    {
        if (obj_name != STRING_EMPTY)
        {
            m_ObjectNameOrigin = obj_name;
            
            bool is_ai = g_Game.IsKindOf(obj_name, &quot;DZ_LightAI&quot;);
            
            PluginDeveloper module_dev = PluginDeveloper.Cast(GetPlugin(PluginDeveloper));
            EntityAI e = module_dev.SpawnEntityOnGroundPos(PluginSceneManager.PLAYER, obj_name, 100, 0.0, pos);
            
            if (e != NULL)
            {
                if (e.IsInherited(ItemBase)) 
                {       
                    ItemBase item = ItemBase.Cast(e);
                    if (item.HasQuantity())
                        item.SetQuantity(item.GetQuantityMax());
                }
                
                m_ObjectName = e.GetType();
                LinkEntityAI(e);
            }
            else if (obj_name != &quot;player&quot;)
            {
                return NULL;
            }
        }
        
        m_LinkedSceneObjects = new array<SceneObject>;
        m_LinkedSceneObjectsShapes = new map<SceneObject, Shape>;
        m_LinkedSceneObjectsIndices = new array<int>;
    
        return this;
