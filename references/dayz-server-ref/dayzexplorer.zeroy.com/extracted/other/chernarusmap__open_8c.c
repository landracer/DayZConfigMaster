// ======================================================================
// File: chernarusmap__open_8c_source.html
// Category: other
// ======================================================================

class ChernarusMap extends ItemMap
{
override void SetActions()
{
super.SetActions();
AddAction(ActionUnfoldMap);
}
override bool GetActionWidgetOverride(out typename name)
{
if ( GetMapStateAnimation() ) //is map in the &#39;opened&#39; state
{
name = ActionFoldMap;
return true;
}
return false;
}
}
class ChernarusMap_Open extends ItemMap {} //unused