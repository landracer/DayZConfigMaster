// ======================================================================
// File: dbgui_8c_source.html
// Category: other
// ======================================================================

class DbgUI
{
private void DbgUI() {}
private void ~DbgUI() {}
static proto native void DoUnitTest();
static proto native void Text(string label);
static proto native void ColoredText(int color, string label);
static proto void Check(string label, out bool checked);
static proto void Combo(string label, out int selection, TStringArray elems);
static proto void List(string label, out int selection, TStringArray elems);
static proto void SliderFloat(string label, out float value, float min, float max, int pxWidth = 150);
static proto native void Spacer(int height);
static proto native void Panel(string label, int width, int height, int color = 0xaa555555);
static proto native bool Button(string txt, int minWidth = 0);
static proto void InputText(string txt, out string value, int pxWidth = 150);
static proto void InputInt(string txt, out int value, int pxWidth = 150);
static proto void InputFloat(string txt, out float value, int pxWidth = 150);
static proto native void PlotLive(string label, int sizeX, int sizeY, float val, int timeStep = 100, int historySize = 30, int color = 0xFFFFFFFF);
static proto native void SameLine();
static proto native void SameSpot();
static proto native void PushID_Int(int int_id);
static proto native void PushID_Str(string str_id);
static proto native void PopID();
static proto void BeginCleanupScope();
static proto native void EndCleanupScope();
static proto native void Begin(string windowTitle, float x = 0, float y = 0);
static proto native void End();
static bool FloatOverride(string id, inout float value, float min, float max, int precision = 1000, bool sameLine = true)
{
if (sameLine)
DbgUI.SameLine();
bool enable;
DbgUI.PushID_Str(id+"_override");
DbgUI.Check("override", enable);
DbgUI.PopID();
if (enable)
{
DbgUI.SameLine();
float tmp = value * (float)precision;
DbgUI.PushID_Str(id+"_slider");
DbgUI.SliderFloat("", tmp, min * (float)precision, max * (float)precision);
DbgUI.PopID();
tmp = tmp / (float)precision;
DbgUI.SameSpot();
DbgUI.PushID_Str(id+"_slider_text");
DbgUI.Text(tmp.ToString());
DbgUI.PopID();
value = tmp;
return true;
}
return false;
}
};