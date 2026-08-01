// ======================================================================
// File: 2__gamelib_2tools_8c_source.html
// Category: other
// ======================================================================

class ScriptCallQueue
{
proto native void Tick(float timeslice);
proto void Call(func fn, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
proto void CallByName(Class obj, string fnName , Param params = NULL);
proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
proto void CallLaterByName(Class obj, string fnName, int delay = 0, bool repeat = false, Param params = NULL);
proto void Remove(func fn);
proto int GetRemainingTime(func fn);
proto void RemoveByName(Class obj, string fnName);
proto int GetRemainingTimeByName(Class obj, string fnName);
proto native void Clear();
};
class ScriptInvoker
{
proto void Invoke(void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
proto bool Insert(func fn, int flags = EScriptInvokerInsertFlags.IMMEDIATE);
proto bool Remove(func fn, int flags = EScriptInvokerRemoveFlags.ALL);
proto int Count(func fn);
proto native void Clear();
};
enum EScriptInvokerInsertFlags
{
NONE,
IMMEDIATE,
UNIQUE,
}
enum EScriptInvokerRemoveFlags
{
NONE,
ALL,
}
class ScriptCaller
{
private void ScriptCaller();
static proto ScriptCaller Create(func fn);
proto void Init(func fn);
proto void Invoke(void param1 = null, void param2 = null, void param3 = null, void param4 = null, void param5 = null, void param6 = null, void param7 = null, void param8 = null, void param9 = null);
proto bool IsValid();
callerA = ScriptCaller.Create(instanceA.SomeMethod);
callerB = ScriptCaller.Create(instanceA.SomeMethod);
Print(callerA.Equals(callerB)); //! "1"
Print(callerA == callerB); //! "0"
Print(callerA); //! "ScriptCaller callerA = ScriptCaller<87bc2d40>"
Print(callerB); //! "ScriptCaller callerB = ScriptCaller<87bc3600>"
callerA = ScriptCaller.Create(instanceA.SomeMethod);
callerB = ScriptCaller.Create(instanceB.SomeMethod);
Print(callerA.Equals(callerB)); //! "0"
Print(callerA == callerB); //! "0"
Print(callerA); //! "ScriptCaller callerA = ScriptCaller<87bc3c40>"
Print(callerB); //! "ScriptCaller callerB = ScriptCaller<87bc2d40>"
}
@endcode
*/
proto bool Equals(notnull ScriptCaller other);
};