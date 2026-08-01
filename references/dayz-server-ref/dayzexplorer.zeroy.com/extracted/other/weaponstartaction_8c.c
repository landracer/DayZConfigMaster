// ======================================================================
// File: weaponstartaction_8c_source.html
// Category: other
// ======================================================================

class WeaponStartAction extends WeaponStateBase
{
WeaponActions m_action;
int m_actionType;
void WeaponStartAction (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
{
m_action = action;
m_actionType = actionType;
}
override void OnEntry (WeaponEventBase e)
{
super.OnEntry(e);
if (e)
{
if (e.m_player)
{
HumanCommandWeapons hcw = e.m_player.GetCommandModifier_Weapons();
if (hcw)
{
HumanCommandAdditives ad = e.m_player.GetCommandModifier_Additives();
if (ad)
ad.CancelModifier();
hcw.StartAction(m_action, m_actionType);
if (hcw.GetRunningAction() == m_action && hcw.GetRunningActionType() == m_actionType)
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("HCW: playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished()); }
}
else
Error("HCW: NOT playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished());
}
else
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("---: remote playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType)); }
}
else
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("---: warning, no player wants to play A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType)); }
}
}
}
override void OnExit (WeaponEventBase e)
{
super.OnExit(e);
}
};