// ======================================================================
// File: staticguiutils_8c_source.html
// Category: other
// ======================================================================

class StaticGUIUtils
{
static const int IMAGESETGROUP_INVENTORY = 0;
static string VerifyIconImageString(int imageset_group = IMAGESETGROUP_INVENTORY, string icon_name = "")
{
if (icon_name == "")
{
return "set:dayz_inventory image:missing";
}
if ( !icon_name.Contains("image:") )
{
switch (imageset_group)
{
case IMAGESETGROUP_INVENTORY:
return "set:dayz_inventory image:" + icon_name;
}
}
return icon_name;
}
}