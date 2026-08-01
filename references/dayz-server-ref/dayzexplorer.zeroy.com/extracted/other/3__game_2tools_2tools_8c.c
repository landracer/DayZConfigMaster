// ======================================================================
// File: 3__game_2tools_2tools_8c_source.html
// Category: other
// ======================================================================

//--------------------------------------------------------------------------
const int CALL_CATEGORY_SYSTEM = 0; // Runs always
const int CALL_CATEGORY_GUI = 1; // Runs always (on client)
const int CALL_CATEGORY_GAMEPLAY = 2; // Runs unless ingame menu is opened
const int CALL_CATEGORY_COUNT = 3;
// -------------------------------------------------------------------------
class CallQueueContext
{
Class m_target;
string m_function;
ref Param m_params;
bool m_valid;
void CallQueueContext(Class target, string fn, Param params)
{
m_target = target;
m_function = fn;
m_params = params;
m_valid = true;
}
void Call()
{
CallParams(m_params);
}
void CallParams(Param params)
{
if (params)
{
g_Game.GameScript.CallFunctionParams(m_target, m_function, NULL, params);
}
else
{
g_Game.GameScript.CallFunction(m_target, m_function, NULL, 0);
}
}
void Invalidate() {
m_valid = false;
}
bool IsValid(){
return m_valid;
}
};
//--------------------------------------------------------------------------
class CallQueue extends array<ref CallQueueContext>
{
private bool m_processing;
void CallQueue()
{
m_processing = false;
}
void Tick()
{
if (m_processing) return;
m_processing = true;
while(Count() > 0)
{
CallQueueContext ctx = Get(0);
if (!ctx.IsValid())
{
Remove(0);
}
else
{
Remove(0);
ctx.Call();
}
}
m_processing = false;
}
void Call(Class obj, string fn_name, Param params = NULL)
{
Insert(new CallQueueContext(obj, fn_name, params));
}
void RemoveCalls(Class obj)
{
if (Count())
{
for (int i = Count() - 1; i >= 0; i--)
{
CallQueueContext ctx = Get(i);
if (ctx.m_target == obj)
{
ctx.Invalidate();
}
}
}
}
};
//--------------------------------------------------------------------------
class DragQueue extends CallQueue
{
private ref Param3<int, int, bool> m_mouse_params; // x,y, is_holding_mb
void DragQueue()
{
m_mouse_params = new Param3<int, int, bool>(0,0,true);
}
override void Tick()
{
if (m_processing) return;
m_processing = true;
int last_index = 0;
int mouse_x;
int mouse_y;
bool is_holding = false;
CallQueueContext ctx;
if (GetMouseState(MouseState.LEFT) & 0x80000000)
{
is_holding = true;
}
GetMousePos(mouse_x, mouse_y);
if (!is_holding || mouse_x != m_mouse_params.param1 || mouse_y != m_mouse_params.param2)
{
m_mouse_params.param1 = mouse_x;
m_mouse_params.param2 = mouse_y;
m_mouse_params.param3 = is_holding;
while (Count() > last_index)
{
ctx = Get(last_index);
if (!ctx.IsValid())
{
Remove(last_index);
}
else
{
ctx.CallParams(m_mouse_params);
last_index++;
}
}
}
// clear queue when mouse button is released
if (!is_holding)
{
Clear();
}
m_processing = false;
}
}
//--------------------------------------------------------------------------
class TimerBase: Managed
{
protected bool m_running;
protected bool m_loop;
protected float m_duration;
protected float m_time;
protected array<TimerBase> m_timerQueue;
protected float m_RunTime;
void ~TimerBase()
{
if (!m_timerQueue) return;
SetRunning(false);
}
void Pause()
{
SetRunning(false);
}
void Continue()
{
SetRunning(true);
}
void Stop()
{
SetRunning(false);
m_time = 0;
}
bool IsRunning()
{
return m_running;
}
void Tick(float timeslice)
{
if (IsRunning())
{
m_RunTime += timeslice;
m_time = m_time + timeslice;
if (m_time >= m_duration)
{
if (m_loop)
{
m_time = m_time - m_duration;
}
else
{
SetRunning(false);
m_time = 0;
}
OnTimer();
}
else
{
OnUpdate();
}
}
}
void OnTimerQueueDestoryed()
{
m_timerQueue = NULL;
}
float GetTime()
{
return m_time;
}
float GetDuration()
{
return m_duration;
}
float GetRemaining()
{
return m_duration - m_time;
}
float GetRunTime()
{
return m_RunTime;
}
protected void OnInit(int category)
{
m_duration = 1;
m_loop = false;
m_time = 0;
m_running = false;
if (g_Game)
m_timerQueue = g_Game.GetTimerQueue(category);
else
ErrorEx("Attempting to Init a timer when the game does not exist (g_Game == null)");
}
protected void OnStart(float duration, bool loop)
{
m_RunTime = 0;
m_duration = duration;
m_loop = loop;
m_time = 0;
SetRunning(true);
}
protected void OnUpdate() {}
protected void OnTimer() {}
protected void SetRunning(bool running)
{
int index = -1;
if (m_running == running) return;
m_running = running;
if (m_timerQueue == NULL) return;
if (running)
{
if (m_timerQueue.Find(this) == -1)
{
m_timerQueue.Insert(this);
}
}
else
{
index = m_timerQueue.Find(this);
if (index != -1)
{
m_timerQueue.Remove(index);
}
}
}
};
//--------------------------------------------------------------------------
class TimerQueue extends array<TimerBase>
{
private bool m_processing;
// -------------------------------------------------------------------------
void TimerQueue()
{
m_processing = false;
}
// -------------------------------------------------------------------------
void ~TimerQueue()
{
if (Count())
{
for (int i = Count() - 1; i >= 0; i--)
{
Get(i).OnTimerQueueDestoryed();
}
Clear();
}
}
// -------------------------------------------------------------------------
void Tick(float timeslice)
{
if (m_processing) return;
m_processing = true;
int count = Count();
for (int i = count - 1; i >= 0; i--)
{
Get(i).Tick(timeslice);
}
m_processing = false;
}
};
//--------------------------------------------------------------------------
class WidgetFadeTimer extends TimerBase
{
private Widget m_widget;
bool m_fadeIn;
float m_alpha;
void WidgetFadeTimer()
{
OnInit(CALL_CATEGORY_GUI);
m_fadeIn = true;
}
void FadeIn(Widget w, float time, bool continue_ = false)
{
m_alpha = w.GetAlpha();
if (continue_ && m_alpha > 0.95)
{
w.SetAlpha(1.0);
w.Show(true);
return;
}
m_widget = w;
m_fadeIn = true;
OnStart(time, false);
if (m_widget)
{
m_alpha = m_widget.GetAlpha();
m_widget.SetAlpha(0);
m_widget.Show(true);
}
if (continue_)
{
m_time = m_alpha * time;
}
}
void FadeOut(Widget w, float time, bool continue_ = false)
{
m_alpha = w.GetAlpha();
if (continue_ && m_alpha < 0.05)
{
w.SetAlpha(0);
w.Show(false);
return;
}
m_widget = w;
m_fadeIn = false;
OnStart(time, false);
if (m_widget && !continue_)
{
m_alpha = 1.0;
m_widget.SetAlpha(m_alpha);
m_widget.Show(true);
}
if (continue_)
{
m_time = (1.0 - m_alpha) * time;
}
}
override private void OnTimer()
{
if (m_widget)
{
if (m_fadeIn)
{
m_widget.SetAlpha(1);
}
else
{
m_widget.SetAlpha(0);
m_widget.Show(false);
}
}
}
override private void OnUpdate()
{
float timeDiff = m_time / m_duration;
float progress;
if (m_widget)
{
if (m_fadeIn)
{
progress = timeDiff;
}
else
{
progress = Math.Lerp(m_alpha,0,timeDiff);
progress = Math.Clamp(progress,0,1);
}
m_widget.SetAlpha(progress);
}
}
};
//--------------------------------------------------------------------------
class Timer extends TimerBase
{
protected Managed m_target;
protected string m_function;
protected ref Param m_params;
void Timer(int category = CALL_CATEGORY_SYSTEM)
{
OnInit(category);
}
void Run(float duration, Managed obj, string fn_name, Param params = NULL, bool loop = false)
{
m_target = obj;
m_function = fn_name;
m_params = params;
OnStart(duration, loop);
}
override protected void OnTimer()
{
if (m_params)
{
g_Game.GameScript.CallFunctionParams(m_target, m_function, NULL, m_params);
m_params = NULL;
}
else
{
g_Game.GameScript.CallFunction(m_target, m_function, NULL, 0);
}
}
override void Stop()
{
super.Stop();
m_params = NULL;
}
};
//--------------------------------------------------------------------------
class AnimationTimer : TimerBase
{
private bool m_Active;
private float m_TargetValue;
private float m_TargetValueOriginal;
private float m_Value;
protected Managed m_TargetObject;
protected string m_UpdateFunction;
protected string m_FinishedFunction;
protected ref Param m_Params;
void AnimationTimer(int category = CALL_CATEGORY_SYSTEM)
{
OnInit(category);
}
void ~AnimationTimer()
{
SetRunning(false);
}
void Run(float targetVal, Managed obj, string updateFunc, string finishedFunc, float startingVal = 0, bool loop = false, float speed = 1.0, Param params = null, int category = CALL_CATEGORY_SYSTEM)
{
SetRunning(true);
m_TargetObject = obj;
m_UpdateFunction = updateFunc;
m_FinishedFunction = finishedFunc;
m_TargetValueOriginal = targetVal;
m_TargetValue = targetVal;
m_time = speed;
m_loop = loop;
m_Active = true;
m_Params = params;
m_Value = startingVal;
}
float GetValue() {
return m_Value;
}
override bool IsRunning()
{
return m_Active;
}
override void Tick(float timeslice)
{
if ( !m_Active )
return;
float diff = Math.AbsFloat(m_TargetValue - m_Value);
float step = m_time * timeslice;
if (diff < step)
{
m_Value = m_TargetValue;
if (!m_loop)
{
m_Active = false;
}
else
{
if (m_TargetValue == m_TargetValueOriginal)
{
m_TargetValue = 0;
}
else
{
m_TargetValue = m_TargetValueOriginal;
}
}
g_Game.GameScript.CallFunction(m_TargetObject, m_FinishedFunction, NULL, m_Params);
}
else
{
if (m_TargetValue > m_Value)
{
m_Value += step;
}
else
{
m_Value -= step;
}
}
g_Game.GameScript.CallFunction(m_TargetObject, m_UpdateFunction, NULL, m_Params);
}
};
class AnimatorTimer
{
private bool m_active = false;
private bool m_loop = false;
private float m_target_value = 0;
private float m_value = 0;
private float m_time = 0;
void Animate(float val, float speed = 1.0)
{
m_target_value = val;
m_loop = false;
m_time = speed;
m_active = true;
}
void AnimateLoop(float speed = 1.0)
{
m_value = 0;
m_target_value = 0;
m_loop = true;
m_time = speed;
m_active = true;
}
float GetValue() {
return m_value;
}
float GetTargetValue() {
return m_target_value;
}
void SetValue(float val) {
m_value = val;
m_target_value = val;
}
bool IsRunning()
{
return m_active;
}
void Tick(float timeslice)
{
if ( !m_active ) return;
if (m_loop)
{
m_target_value += m_time * Math.PI2 * timeslice;
while (m_target_value > Math.PI2) m_target_value -= Math.PI2;
m_value = Math.Sin(m_target_value);
}
else
{
float diff = Math.AbsFloat(m_target_value - m_value);
float step = m_time * timeslice;
if (diff < step)
{
m_value = m_target_value;
m_active = false;
}
else
{
if (m_target_value > m_value)
{
m_value += step;
}
else
{
m_value -= step;
}
}
}
}
};
class multiMap<Class K, Class V>
{
private ref array<ref array<V> > m_values;
private ref array<K> m_keys;
bool HasKey(K key)
{
int index = -1;
if (m_keys)
{
index = m_keys.Find(key);
}
if (index != -1)
{
return true;
}
return false;
}
array<V> Get(K key)
{
int index = -1;
if (m_keys)
{
index = m_keys.Find(key);
}
if (index != -1)
{
return m_values.Get(index);
}
return NULL;
}
array<V> GetByIndex(int index)
{
return m_values.Get(index);
}
K GetKeyByIndex(int index)
{
return m_keys.Get(index);
}
void Insert(K key, V value)
{
int index = -1;
if (!m_keys)
{
m_keys = new array<K>;
m_values = new array<ref array<V> >;
}
else
{
index = m_keys.Find(key);
}
if (index == -1)
{
array<V> value_array = new array<V>;
value_array.Insert(value);
m_keys.Insert(key);
m_values.Insert(value_array);
}
else
{
m_values.Get(index).Insert(value);
}
}
void RemoveByIndex(int index)
{
m_keys.Remove(index);
m_values.Remove(index);
}
void Remove(K key)
{
int index = -1;
if (m_keys)
{
index = m_keys.Find(key);
}
if (index != -1)
{
RemoveByIndex(index);
}
}
int Count()
{
if (m_keys)
{
return m_keys.Count();
}
return 0;
}
void Clear()
{
if ( m_keys && m_values)
{
m_keys.Clear();
m_values.Clear();
}
}
void ~multiMap()
{
Clear();
}
};
// at last one template definition should be here, for template initialization in this script module
typedef map<string, string> TStringMap;
int GetTemperatureColor(int temperature)
{
int alpha = 255;
int red = 153;
int green = 153;
int blue = 153;
if (temperature < GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_LOWER_LIMIT)
{
temperature = temperature - 20;
temperature = Math.Clamp( temperature, -50, 50);
temperature = Math.AbsInt(temperature);
red = Math.Clamp ( red - ((red/50 )*temperature), 0, 255);
green = Math.Clamp ( green - ((green/50 )*temperature), 0, 255);
blue = Math.Clamp ( blue+((blue/50)*temperature), 0, 255);
}
else if (temperature > GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_UPPER_LIMIT)
{
temperature = Math.Clamp(temperature, -100, 100);
blue = Math.Clamp (blue - ((blue / 100) * temperature), 0, 255);
green = Math.Clamp (green - ((green / 100) * temperature), 0, 255);
red = Math.Clamp (red + ((red / 100) * temperature), 0, 255);
}
return ARGB(alpha, red, green, blue);
}
bool GetProfileValueBool(string name, bool def = false)
{
string value;
if (g_Game.GetProfileString(name, value))
{
if (value == "true" || value == "1")
{
return true;
}
else
{
return false;
}
}
else
{
return def;
}
}
void SetProfileValueBool(string name, bool value)
{
if (value)
{
g_Game.SetProfileString(name, "1");
}
else
{
g_Game.SetProfileString(name, "0");
}
}
int GetNumberOfSetBits(int i)//leaving here for legacy/modding reasons
{
return Math.GetNumberOfSetBits(i);
}