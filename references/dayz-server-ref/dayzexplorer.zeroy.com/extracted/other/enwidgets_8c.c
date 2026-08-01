// ======================================================================
// File: enwidgets_8c_source.html
// Category: other
// ======================================================================

TextWidgetTypeID,
MultilineTextWidgetTypeID,
MultilineEditBoxWidgetTypeID,
RichTextWidgetTypeID,
RenderTargetWidgetTypeID,
ImageWidgetTypeID,
ConsoleWidgetTypeID,
VideoWidgetTypeID,
RTTextureWidgetTypeID,
FrameWidgetTypeID,
EmbededWidgetTypeID,
ButtonWidgetTypeID,
CheckBoxWidgetTypeID,
WindowWidgetTypeID,
ComboBoxWidgetTypeID,
SimpleProgressBarWidgetTypeID,
ProgressBarWidgetTypeID,
SliderWidgetTypeID,
BaseListboxWidgetTypeID,
TextListboxWidgetTypeID,
GenericListboxWidgetTypeID,
EditBoxWidgetTypeID,
PasswordEditBoxWidgetTypeID,
WorkspaceWidgetTypeID,
GridSpacerWidgetTypeID,
WrapSpacerWidgetTypeID,
ScrollWidgetTypeID,
#else
typedef TypeID WidgetType;
#endif
typedef TypeID EventType;
enum WidgetFlags
{
SOURCEALPHA, //< takes alpha from texture * alpha from color. If not set, considers texture as non-transparent and transparency is set only by color
BLEND, //< texture is blended with a surface base on alpha
ADDITIVE, //< texture is added to a surface based on alpha
VISIBLE, //<Widget is visible - ShowWidget(w, true)
NOWRAP, //< Do not do texture wrapping
CENTER, //< Centers TextWidgetTypeID
VCENTER, //< Centers TextWidgetTypeID verticaly
HEXACTPOS,
VEXACTPOS,
EXACTPOS, //< Uses physical resolution (g_iWidth, h_iHeight)
HEXACTSIZE, //< Uses physical resolution (g_iWidth)
VEXACTSIZE, //< Uses physical resolution (h_iHeight)
EXACTSIZE, //< Uses physical resolution
NOFILTER, //< no texture filtering (no blur)
RALIGN, //< Right alignment TextWidgetTypeID
STRETCH, //< Stretch texture to a full size
FLIPU, //< Flips texture in U axis
FLIPV, //< Flips texture in V axis
CUSTOMUV, //< ignores STRETCH/FLIPU/FLIPV and take custom UV set by SetWidgetUV()
IGNOREPOINTER,
DISABLED,
NOFOCUS,
CLIPCHILDREN,
RENDER_ALWAYS,
NOCLEAR,
DRAGGABLE
};
//------------------------------------------
enum WidgetAlignment
{
WA_LEFT = 0,
WA_RIGHT = 1,
WA_CENTER = 2,
WA_TOP = 0,
WA_BOTTOM = 1,
}
//------------------------------------------
/*enum LinebreakOverrideMode
{
LINEBREAK_DEFAULT,
LINEBREAK_WESTERN,
LINEBREAK_ASIAN
};*/
//------------------------------------------
class Widget: Managed
{
proto void ~Widget();
proto private void Widget();
proto static string TranslateString(string stringId);
proto static void SetLV(float lv);
proto static void SetTextLV(float lv);
proto static void SetObjectLighting(float lighting);
proto native owned string GetName();
proto native void SetName(string name);
proto native owned string GetTypeName();
proto native WidgetType GetTypeID();
proto native void Show(bool show, bool immedUpdate = true);
proto native void Enable(bool enable);
proto native int GetFlags();
proto native int SetFlags(int flags, bool immedUpdate = true);
proto native int GetSort();
proto native void SetSort(int sort, bool immedUpdate = true);
proto native int ClearFlags(int flags, bool immedUpdate = true);
proto native bool IsControlClass();
proto native owned string GetStyleName();
proto void GetUserData(out Class data);
proto native void SetUserData(Class data);
proto native int GetUserID();
proto native void SetUserID(int id);
proto native bool IsVisible();
proto native bool IsVisibleHierarchy();
proto native void SetPos(float x, float y, bool immedUpdate = true);
proto native void SetSize(float w, float h, bool immedUpdate = true);
proto native void SetScreenPos(float x, float y, bool immedUpdate = true);
proto native void SetScreenSize(float w, float h, bool immedUpdate = true);
proto native void SetColor(int color);
proto native int GetColor();
proto native void SetRotation(float roll, float pitch, float yaw, bool immedUpdate = true);
proto native vector GetRotation();
proto native void SetAlpha(float alpha);
proto native float GetAlpha();
proto void GetPos(out float x, out float y);
proto void GetSize(out float width, out float height);
proto void GetScreenPos(out float x, out float y);
proto void GetScreenSize(out float width, out float height);
proto native void SetTransform(vector mat[4], bool immedUpdate = true);
proto native Widget GetParent();
proto native Widget GetChildren();
proto native Widget GetSibling();
proto native void AddChild(Widget child, bool immedUpdate = true);
proto native void RemoveChild(Widget child);
proto native volatile void Update();
proto void GetScript(out Class data);
proto native Widget FindWidget(string pathname); //find Widget by path. e.g FindWidget("widget1.widget2.widget3.mywidget")
proto native Widget FindAnyWidget(string pathname); //find Widget by name e.g. FindWidget("widget1")
proto native Widget FindAnyWidgetById(int user_id); //find Widget by userID
proto native void SetHandler(ScriptedWidgetEventHandler eventHandler);
proto native void Unlink(); //destroys widget and all its children
};
class WorkspaceWidget: Widget
{
proto native external Widget CreateWidget(WidgetType type, int left, int top, int width, int height, WidgetFlags flags, int color, int sort, Widget parentWidget = NULL);
proto native external Widget CreateWidgets(string layout, Widget parentWidget = NULL, bool immedUpdate = true);
};
proto native Widget GetWidgetUnderCursor();
proto native Widget CancelWidgetDragging();
proto native Widget GetDragWidget();
proto native void ReportMouse(int mousex, int mousey, Widget rootWidget);
class TextWidget extends Widget
{
proto native void SetTextSpacing(int horiz, int vert);
proto native void SetTextExactSize(int size);
proto native void SetTextOffset(int left, int top);
proto native void SetText(string text, bool immedUpdate = true);
proto native void SetOutline(int outlineSize, int argb = 0xFF000000);
proto native int GetOutlineSize();
proto native int GetOutlineColor();
proto native void SetShadow(int shadowSize, int shadowARGB = 0xFF000000, float shadowOpacity = 1, float shadowOffsetX = 0, float shadowOffsetY = 0);
proto native int GetShadowSize();
proto native int GetShadowColor();
proto native float GetShadowOpacity();
proto void GetShadowOffset(out float sx, out float sy);
proto native void SetItalic(bool italic);
proto native bool GetItalic();
proto native void SetBold(bool bold);
proto native bool GetBold();
proto void GetTextSize(out int sx, out int sy);
proto void SetTextFormat(string text, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
proto native float GetTextProportion();
proto native void SetTextProportion(float val);
};
class MultilineTextWidget extends TextWidget
{
proto native float SetLineBreakingOverride(int mode);
};
class RichTextWidget extends TextWidget
{
proto native float GetContentHeight();
proto native float GetContentOffset();
proto native void SetContentOffset(float offset, bool snapToLine = false);
proto native void ElideText(int line, float maxWidth, string str);
proto native int GetNumLines();
proto native void SetLinesVisibility(int lineFrom, int lineTo, bool visible);
proto native float GetLineWidth(int line);
proto native float SetLineBreakingOverride(int mode);
};
class RenderTargetWidget extends Widget
{
proto native void SetRefresh(int period, int offset);
proto native void SetResolutionScale(float xscale, float ycale);
};
class RTTextureWidget extends Widget
{
};
class ImageWidget extends Widget
{
proto native bool LoadImageFile(int num, string name, bool noCache = false);
proto native void SetImageTexture(int image, RTTextureWidget texture);
proto void GetImageSize(int image, out int sx, out int sy);
proto native bool SetImage(int num);
proto native int GetImage();
proto native void SetUV(float uv[4][2]);
proto native bool LoadMaskTexture(string resource);
proto native float GetMaskProgress();
proto native void SetMaskProgress(float value);
proto native float GetMaskTransitionWidth();
proto native void SetMaskTransitionWidth(float value);
};
class MultilineEditBoxWidget extends TextWidget
{
proto native int GetLinesCount();
proto native int GetCarriageLine();
proto native int GetCarriagePos();
proto void GetText(out string text);
proto native void SetLine(int line, string text);
proto void GetLine(int line, out string text);
};
class UIWidget extends Widget
{
proto native void SetTextColor(int color);
proto native void SetTextOutline(int outlineSize, int argb = 0xFF000000);
proto native int GetTextOutlineSize();
proto native int GetTextOutlineColor();
proto native void SetTextShadow(int shadowSize, int shadowARGB = 0xFF000000, float shadowOpacity = 1.0, float shadowOffsetX = 0.0, float shadowOffsetY = 0.0);
proto native int GetTextShadowSize();
proto native int GetTextShadowColor();
proto native float GetTextShadowOpacity();
proto native float GetTextShadowOffsetX();
proto native float GetTextShadowOffsetY();
proto native void SetTextItalic(bool italic);
proto native bool GetTextItalic();
proto native void SetTextBold(bool bold);
proto native bool GetTextBold();
};
class CanvasWidget extends Widget
{
proto native void DrawLine(float x1, float y1, float x2, float y2, float width, int color);
proto native void Clear();
};
class EditBoxWidget extends UIWidget
{
proto string GetText();
proto native void SetText(string str);
};
class PasswordEditBoxWidget extends EditBoxWidget
{
proto native void SetHideText(bool hide);
};
class SliderWidget extends UIWidget
{
proto native void SetMinMax(float minimum, float maximum);
proto native float GetMin();
proto native float GetMax();
proto native float GetCurrent();
proto native void SetCurrent(float curr);
proto native float GetStep();
proto native void SetStep(float step);
};
class SimpleProgressBarWidget extends UIWidget
{
proto native float GetMin();
proto native float GetMax();
proto native float GetCurrent();
proto native void SetCurrent(float curr);
};
class ProgressBarWidget extends SimpleProgressBarWidget
{
};
class ButtonWidget extends UIWidget
{
proto native bool GetState();
proto native bool SetState(bool state);
proto native void SetText(string text);
proto void GetText(out string text);
proto native void SetTextOffset(float xoffset, float yoffset);
proto native void SetTextHorizontalAlignment(int align);
proto native void SetTextVerticalAlignment(int align);
proto native float GetTextProportion();
proto native void SetTextProportion(float val);
};
class XComboBoxWidget extends UIWidget
{
proto native int AddItem(string item);
proto native void ClearAll();
proto native void SetItem(int item, string value);
proto native void RemoveItem(int item);
proto native int GetNumItems();
proto native int SetCurrentItem(int n);
proto native int GetCurrentItem();
};
class CheckBoxWidget extends UIWidget
{
proto native void SetText(string str);
proto native bool IsChecked();
proto native void SetChecked(bool checked);
};
class BaseListboxWidget extends UIWidget
{
proto native void ClearItems();
proto native int GetNumItems();
proto native void SelectRow(int row);
proto native int GetSelectedRow();
proto native void RemoveRow(int row);
proto native void EnsureVisible(int row);
};
class SimpleListboxWidget extends BaseListboxWidget
{
};
class TextListboxWidget extends SimpleListboxWidget
{
proto native int AddItem(string text, Class userData, int column, int row = -1);
proto native void SetItem(int position, string text, Class userData, int column);
proto bool GetItemText(int row, int column, out string text);
proto void GetItemData(int row, int column, out Class data);
proto native void SetItemColor(int row, int column, int color );
};
class SpacerBaseWidget extends UIWidget
{
proto native void AddChildAfter(Widget child,Widget after, bool immedUpdate = true);
}
class SpacerWidget extends SpacerBaseWidget
{
proto native WidgetAlignment GetContentAlignmentH();
proto native void SetContentAlignmentH(WidgetAlignment alignment);
proto native WidgetAlignment GetContentAlignmentV();
proto native void SetContentAlignmentV(WidgetAlignment alignment);
}
class GridSpacerWidget extends SpacerWidget
{
}
class WrapSpacerWidget extends SpacerWidget
{
}
class ScrollWidget extends SpacerBaseWidget
{
proto native float GetScrollbarWidth();
proto native bool IsScrollbarVisible(); //reflects native C++ side scrollbar state
proto native float GetContentWidth();
proto native float GetContentHeight();
proto native float GetHScrollPos();
proto native float GetHScrollPos01();
proto native bool HScrollStep(int steps);
proto native void HScrollToPos(float pos);
proto native void HScrollToPos01(float pos01);
proto native void HScrollToWidget(Widget child);
proto native float GetVScrollPos();
proto native float GetVScrollPos01();
proto native bool VScrollStep(int steps);
proto native void VScrollToPos(float pos);
proto native void VScrollToPos01(float pos01);
proto native void VScrollToWidget(Widget child);
};
enum VideoCommand
{
PLAY,
STOP,
REWIND,
POSITION,
REPEAT,
ISPLAYING,
KILL
};
enum VideoState
{
NONE,
PLAYING,
PAUSED,
STOPPED,
FINISHED,
};
enum VideoCallback
{
ON_PLAY,
ON_PAUSE,
ON_STOP,
ON_END,
ON_LOAD,
ON_SEEK,
ON_BUFFERING_START,
ON_BUFFERING_END,
ON_ERROR,
};
class VideoWidget extends Widget
{
proto native bool Load(string name, bool looping = false, int startTime = 0);
proto native void Unload();
proto native bool Play();
proto native bool Pause();
proto native bool Stop();
proto native bool SetTime(int time, bool preload);
proto native int GetTime();
proto native int GetTotalTime();
proto native void SetLooping(bool looping);
proto native bool IsLooping();
proto native bool IsPlaying();
proto native VideoState GetState();
proto native void DisableSubtitles(bool disable);
proto native bool IsSubtitlesDisabled();
proto void SetCallback(VideoCallback cb, func fn);
int Play(VideoCommand cmd)
{
// Yes, some things here do not do what the name implies
// And it returns 0 whether successful or not
// But this is what the old functionality looked like
// So for true backwards compat, it will be left like this
switch (cmd)
{
case VideoCommand.PLAY:
Play();
break;
case VideoCommand.STOP:
Pause();
break;
case VideoCommand.REWIND:
SetTime(0, true);
break;
case VideoCommand.POSITION:
return GetTime();
case VideoCommand.REPEAT:
SetLooping(true);
break;
case VideoCommand.ISPLAYING:
return IsPlaying();
case VideoCommand.KILL:
Unload();
break;
default:
return 0;
}
return 0;
}
bool LoadVideo(string name, int soundScene)
{
return Load(name);
}
};
proto native void SetGUIWidget(IEntity ent, int index, RTTextureWidget w);
enum ControlID
{
CID_NONE = 0,
CID_SELECT = 1,//select/use focused
CID_BACK,
CID_LEFT, //navigation
CID_RIGHT,
CID_UP,
CID_DOWN,
CID_MENU, //get to main menu
CID_DRAG, //probably needed only for consoles
CID_TABLEFT,
CID_TABRIGHT,
CID_RADIALMENU,
CID_COUNT
};
class ScriptedWidgetEventHandler: Managed
{
bool OnClick(Widget w, int x, int y, int button);
bool OnModalResult(Widget w, int x, int y, int code, int result);
bool OnDoubleClick(Widget w, int x, int y, int button);
bool OnSelect(Widget w, int x, int y);
bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn);
bool OnFocus(Widget w, int x, int y);
bool OnFocusLost(Widget w, int x, int y);
bool OnMouseEnter(Widget w, int x, int y);
bool OnMouseLeave(Widget w, Widget enterW, int x, int y);
bool OnMouseWheel(Widget w, int x, int y, int wheel);
bool OnMouseButtonDown(Widget w, int x, int y, int button);
bool OnMouseButtonUp(Widget w, int x, int y, int button);
bool OnController(Widget w, int control, int value);
bool OnKeyDown(Widget w, int x, int y, int key);
bool OnKeyUp(Widget w, int x, int y, int key);
bool OnKeyPress(Widget w, int x, int y, int key);
bool OnChange(Widget w, int x, int y, bool finished);
bool OnDrag(Widget w, int x, int y);
bool OnDragging(Widget w, int x, int y, Widget reciever);
bool OnDraggingOver(Widget w, int x, int y, Widget reciever);
bool OnDrop(Widget w, int x, int y, Widget reciever);
bool OnDropReceived(Widget w, int x, int y, Widget reciever);
bool OnResize(Widget w, int x, int y);
bool OnChildAdd(Widget w, Widget child);
bool OnChildRemove(Widget w, Widget child);
bool OnUpdate(Widget w);
bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1);
};
//Common Widget API
proto native void SetCursorWidget(Widget cursor);
proto native void ShowCursorWidget(bool show);
proto native bool LoadWidgetImageSet(string filename);
proto native void LoadWidgetStyles(string filename);
// sets active window (widget which owns some control inputs like buttons, listboxes etc.). Necessary for controlling the focus by keyboard/joypad. In case when setFocus is set to true, it sets focus on a first child Widget, which may receive the focus (is not disabled, set as NoFocus etc.)
proto native bool SetActiveWindow(Widget w, bool resetFocus);
// sets focus (necessary when using keyboard/joyped) to a particular widget. Widget must have some inputs like button, listbox, checkbox, combobox etc.
proto native void SetFocus(Widget w);
proto native void SetModal(Widget w);
proto native Widget GetFocus();
//RenderTargetWidgetTypeID
proto native void SetWidgetWorld(RenderTargetWidget w, IEntity wrldEntity, int camera);
#ifdef PS3
proto native bool IsCircleToCrossSwapped();
#endif
proto native bool ReloadTexture(string path);