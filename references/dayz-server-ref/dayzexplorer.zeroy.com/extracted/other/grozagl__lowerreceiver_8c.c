// ======================================================================
// File: grozagl__lowerreceiver_8c_source.html
// Category: other
// ======================================================================

class GrozaGL_LowerReceiver extends ItemBase
{
override bool CanPutAsAttachment( EntityAI parent )
{
if(!super.CanPutAsAttachment(parent)) {return false;}
if ( parent.FindAttachmentBySlotName("weaponMuzzleAK") == NULL )
{
return true;
}
return true;
}
}