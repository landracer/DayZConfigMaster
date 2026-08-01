// ======================================================================
// File: m4__carryhandleoptic_8c_source.html
// Category: other
// ======================================================================

class M4_CarryHandleOptic extends ItemOptics
{
override bool CanPutAsAttachment( EntityAI parent )
{
if(!super.CanPutAsAttachment(parent)) {return false;}
if ( parent.IsKindOf("M4A1_Base") )
{
return true;
}
return false;
}
}