// ======================================================================
// File: embeded_8c_source.html
// Category: other
// ======================================================================

class EmbededButtonScript
{
reference string Caption;
void OnWidgetScriptInit(Widget w)
{
ButtonWidget button = ButtonWidget.Cast(w.GetChildren());
button.SetText(Caption);
}
};