// ======================================================================
// File: weaponstablestate_8c_source.html
// Category: other
// ======================================================================

enum MuzzleState
{
U = -1,
E = 0,
F = 1,
L = 2
}
class WeaponStableState extends WeaponStateBase
{
int m_animState;
ref array<MuzzleState> m_muzzleHasBullet = new array<MuzzleState>();
void WeaponStableState(Weapon_Base w = NULL, WeaponStateBase parent = NULL, int anim_state = -1)
{
m_animState = anim_state;
InitMuzzleArray();
ValidateMuzzleArray();
}
void SyncAnimState()
{
int curr = m_weapon.GetWeaponAnimState();
if (curr != m_animState)
{
//if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " synchronizing anim state: " + curr + " --> " + m_animState);
PlayerBase p;
if (Class.CastTo(p, m_weapon.GetHierarchyParent()))
{
if (p.GetItemInHands() == m_weapon)
{
HumanCommandWeapons hcw = p.GetCommandModifier_Weapons();
if (hcw)
{
hcw.SetInitState(m_animState);
if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " state=" + m_weapon.GetCurrentState().Type() + " synchronized anim state: " + typename.EnumToString(PistolAnimState, curr) + " --> " + typename.EnumToString(PistolAnimState, m_animState));
}
else
{
Human wpnOwner = Human.Cast(m_weapon.GetHierarchyRootPlayer());
HumanCommandWeapons.StaticSetInitState(wpnOwner, m_animState);
if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " state=" + m_weapon.GetCurrentState().Type() + " synchronized remote anim state: " + typename.EnumToString(PistolAnimState, curr) + " --> " + typename.EnumToString(PistolAnimState, m_animState));
}
}
m_weapon.SetWeaponAnimState(m_animState);
}
else
{
m_weapon.SetGroundAnimFrameIndex(m_animState);
}
}
}
override void OnEntry(WeaponEventBase e)
{
super.OnEntry(e);
if (e)
SyncAnimState();
m_weapon.SetJammed(IsJammed());
m_weapon.SetCharged(!IsDischarged());
m_weapon.SetWeaponOpen(IsWeaponOpen());
m_weapon.ResetWeaponAnimState();
}
override void OnUpdate(float dt)
{
super.OnUpdate(dt);
SyncAnimState();
}
override void OnExit(WeaponEventBase e)
{
m_weapon.ResetWeaponAnimState();
super.OnExit(e);
}
override bool IsIdle() { return true; }
int GetCurrentStateID() { return 0; }
bool IsRepairEnabled() { return false; }
bool HasBullet() { return false; }
bool HasMagazine() { return false; }
bool IsJammed() { return false; }
bool IsDischarged() { return false; }
bool IsWeaponOpen() { return false; }
void InitMuzzleArray() { m_muzzleHasBullet = { MuzzleState.U }; }
bool IsSingleState() { return false; }
MuzzleState GetMuzzleState(int idx) { return m_muzzleHasBullet[idx]; }
int GetMuzzleStateCount() { return m_muzzleHasBullet.Count(); }
bool IsChamberValid(int idx) { return m_muzzleHasBullet[idx] != MuzzleState.U; }
bool IsChamberFiredOut(int idx) { return m_muzzleHasBullet[idx] == MuzzleState.F; }
bool IsChamberFull(int idx) { return m_muzzleHasBullet[idx] != MuzzleState.E; }
void ValidateMuzzleArray()
{
// There is only one
if (IsSingleState())
return;
bool failed = false;
if (!failed)
{
foreach (MuzzleState state : m_muzzleHasBullet)
{
if ( state == MuzzleState.U )
{
failed = true;
break;
}
}
}
if (failed)
{
ErrorExSeverity severity = ErrorExSeverity.ERROR;
#ifdef DEVELOPER
if (DayZGame.m_IsPreviewSpawn)
{
// we do not want VMEs when spawning the item in order to visualize it for preview in script console
severity = ErrorExSeverity.INFO;
}
#endif
ErrorEx("Muzzle array validation has failed. Please set up the correct muzzle states by overriding InitMuzzleArray.", severity);
}
}
};