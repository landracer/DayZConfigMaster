// ======================================================================
// File: aigroup_8c_source.html
// Category: other
// ======================================================================

class AIGroup : Managed
{
private void AIGroup();
private void ~AIGroup();
proto native void AddAgent(notnull AIAgent agent);
proto native void RemoveAgent(notnull AIAgent agent);
proto native AIGroupBehaviour GetBehaviour();
};