// ======================================================================
// File: groza__barrel__suppressor_8c_source.html
// Category: other
// ======================================================================

class Groza_Barrel_Suppressor extends SuppressorBase
{
override bool CanPutAsAttachment( EntityAI parent )
{
if(!super.CanPutAsAttachment(parent)) {return false;}
if ( !parent.FindAttachmentBySlotName("weaponButtstockAK").IsKindOf("GrozaGL_LowerReceiver") )
{
return true;
}
return false;
}
}