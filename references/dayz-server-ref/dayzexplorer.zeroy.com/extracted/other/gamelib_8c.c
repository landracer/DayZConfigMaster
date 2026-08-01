// ======================================================================
// File: gamelib_8c_source.html
// Category: other
// ======================================================================

#ifdef GAME_TEMPLATE
Game g_Game;
Game GetGame()
{
return g_Game;
}
class Game
{
ScriptModule GameScript;
ScriptModule GetScriptModule()
{
return GameScript;
}
void SetDebug(bool isDebug) {}
void OnEvent(EventType eventTypeId, Param params)
{
Print("OnEvent");
}
void OnAfterInit()
{
Print("OnAfterInit");
}
void OnUpdate(float timeslice)
{
}
proto native bool SetWorldFile(string path, bool reload);
proto native owned string GetWorldFile();
bool OnGameStart()
{
return true;
}
void OnGameEnd()
{
}
void ShowLoadingAnim()
{
}
void HideLoadingAnim()
{
}
void UpdateLoadingAnim(float timeslice, float progress)
{
}
proto native IEntity SpawnEntity(typename typeName);
proto native IEntity SpawnEntityTemplate(vobject templateResource);
proto native GenericComponent SpawnComponentTemplate(IEntity owner, vobject templateResource);
proto native IEntity FindEntity(string name);
proto native WorkspaceWidget GetWorkspace();
proto native void RequestClose();
proto native void RequestReload();
proto native owned string GetBuildVersion();
proto native owned string GetBuildTime();
proto native GenericWorldEntity GetWorldEntity();
proto native InputManager GetInputManager();
proto native MenuManager GetMenuManager();
proto native int GetTickCount();
}
void GameLibInit()
{
}
#endif