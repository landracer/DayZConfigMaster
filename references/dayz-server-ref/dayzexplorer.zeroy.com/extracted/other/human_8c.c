// ======================================================================
// File: human_8c_source.html
// Category: other
// ======================================================================

enum HumanInputControllerOverrideType
{
DISABLED,
ENABLED,
ONE_FRAME,
};
// *************************************************************************************
// ! HumanInputController - what we know about the input - human.GetInputController()
// *************************************************************************************
class HumanInputController
{
//--------------------------------------------------------------
proto native void SetDisabled(bool pState);
proto void GetMovement(out float pSpeed, out vector pLocalDirection);
proto native float GetHeadingAngle();
proto native vector GetAimChange();
proto native vector GetAimDelta(float dt);
proto native vector GetTracking();
proto native bool CameraViewChanged();
proto native bool CameraIsFreeLook();
proto native void ResetFreeLookToggle();
proto native bool CameraIsTracking();
proto native bool Camera3rdIsRightShoulder();
//--------------------------------------------------------------
proto native bool IsStanceChange();
proto native bool IsJumpClimb();
//--------------------------------------------------------------
proto native bool IsMeleeEvade();
proto native bool IsMeleeFastAttackModifier();
proto native int IsMeleeLREvade();
proto native bool IsMeleeWeaponAttack();
//--------------------------------------------------------------
proto native bool WeaponWasRaiseClick();
proto native bool IsWeaponRaised();
proto native bool WeaponADS();
proto native void ResetADS();
proto native bool IsThrowingModeChange();
proto native void ResetThrowingMode();
proto native bool IsWalkToggled();
//--------------------------------------------------------------
proto native bool IsUseButton();
proto native bool IsUseButtonDown();
proto native bool IsUseItemButton();
proto native bool IsUseItemButtonDown();
proto native bool IsAttackButton();
proto native bool IsAttackButtonDown();
proto native bool IsSingleUse();
proto native bool IsContinuousUse();
proto native bool IsContinuousUseStart();
proto native bool IsContinuousUseEnd();
proto native bool IsImmediateAction(); //TODO: revise, may be obsolete
//--------------------------------------------------------------
proto native bool IsReloadOrMechanismSingleUse();
proto native bool IsReloadOrMechanismContinuousUse();
proto native bool IsReloadOrMechanismContinuousUseStart();
proto native bool IsReloadOrMechanismContinuousUseEnd();
proto native bool IsZoom();
proto native bool IsZoomToggle();
proto native void ResetZoomToggle();
proto native bool IsSightChange();
proto native bool IsZoomIn();
proto native bool IsZoomOut();
proto native bool IsFireModeChange();
proto native bool IsZeroingUp();
proto native bool IsZeroingDown();
proto native bool IsHoldBreath();
proto native void ResetHoldBreath();
//--------------------------------------------------------------
proto native int IsGestureSlot();
proto native bool IsOtherController();
//--------------------------------------------------------------
proto native int IsQuickBarSlot();
proto native bool IsQuickBarSingleUse();
proto native bool IsQuickBarContinuousUse();
proto native bool IsQuickBarContinuousUseStart();
proto native bool IsQuickBarContinuousUseEnd();
//--------------------------------------------------------------
// LIMITS
proto native void LimitsDisableSprint(bool pDisable);
proto native bool LimitsIsSprintDisabled();
//--------------------------------------------------------------
// overrides
proto native void OverrideMovementSpeed(HumanInputControllerOverrideType overrideType, float value);
proto native void OverrideMovementAngle(HumanInputControllerOverrideType overrideType, float value);
proto native void OverrideAimChangeX(HumanInputControllerOverrideType overrideType, float value);
proto native void OverrideAimChangeY(HumanInputControllerOverrideType overrideType, float value);
proto native void OverrideMeleeEvade(HumanInputControllerOverrideType overrideType, bool value);
proto native void OverrideRaise(HumanInputControllerOverrideType overrideType, bool value);
proto native void Override3rdIsRightShoulder(HumanInputControllerOverrideType overrideType, bool value);
proto native void OverrideFreeLook(HumanInputControllerOverrideType overrideType, bool value);
//--------------------------------------------------------------
private void HumanInputController()
{
}
private void ~HumanInputController()
{
}
}
// *************************************************************************************
// ! HumanInputController - what we know about the input - human.GetInputController()
// *************************************************************************************
typedef int TAnimGraphCommand;
typedef int TAnimGraphVariable;
typedef int TAnimGraphTag;
typedef int TAnimGraphEvent;
class HumanAnimInterface
{
private void HumanAnimInterface();
private void ~HumanAnimInterface();
//-----------------------------------------------------
// Binds, returns -1 when error, otherwise if ok
proto native TAnimGraphCommand BindCommand(string pCommandName);
proto native TAnimGraphVariable BindVariableFloat(string pVariable);
proto native TAnimGraphVariable BindVariableInt(string pVariable);
proto native TAnimGraphVariable BindVariableBool(string pVariable);
proto native TAnimGraphTag BindTag(string pTagName);
proto native TAnimGraphEvent BindEvent(string pEventName);
}
// *************************************************************************************
// ! HumanCommandActionCallback - called as action callback
// *************************************************************************************
class HumanCommandActionCallback
{
private void HumanCommandActionCallback();
private void ~HumanCommandActionCallback();
proto native Human GetHuman();
proto native void Cancel();
proto native void InternalCommand(int pInternalCommandId);
proto native void SetAligning(vector pPositionWS, vector pDirectionWS);
proto native void ResetAligning();
proto native void EnableCancelCondition(bool pEnable);
proto native bool DefaultCancelCondition();
proto native void RegisterAnimationEvent(string pAnimationEventStr, int pId);
proto native void EnableStateChangeCallback();
// bool CancelCondition() { return false; }
static const int STATE_NONE = 0;
static const int STATE_LOOP_IN = 1;
static const int STATE_LOOP_LOOP = 2;
static const int STATE_LOOP_END = 3;
static const int STATE_LOOP_END2 = 4;
static const int STATE_LOOP_LOOP2 = 5;
static const int STATE_LOOP_ACTION = 6;
static const int STATE_NORMAL = 7;
proto native int GetState();
//----------------------------------
// debug helper
static string GetStateString(int pState)
{
switch (pState)
{
case STATE_NONE: return "NONE";
case STATE_LOOP_IN: return "LOOP_IN";
case STATE_LOOP_LOOP: return "LOOP_LOOP";
case STATE_LOOP_END: return "LOOP_END";
case STATE_LOOP_END2: return "LOOP_END2";
case STATE_LOOP_LOOP2: return "LOOP_LOOP2";
case STATE_LOOP_ACTION: return "LOOP_ACTION";
case STATE_NORMAL: return "ONE TIME";
}
return "Undef";
}
string GetStateString()
{
return GetStateString(GetState());
}
//----------------------------------
// callbacks
void OnAnimationEvent(int pEventID) {};
void OnFinish(bool pCanceled) {};
void OnStateChange(int pOldState, int pCurrentState) {};
// helps identify type of callback
bool IsUserActionCallback()
{
return false;
}
bool IsGestureCallback()
{
return false;
}
bool IsSymptomCallback()
{
return false;
}
}
// *************************************************************************************
// ! HumanCommandMove
// *************************************************************************************
class HumanCommandMove
{
private void HumanCommandMove() {}
private void ~HumanCommandMove() {}
proto native float GetCurrentMovementAngle();
proto bool GetCurrentInputAngle(out float pAngle);
proto native float GetCurrentMovementSpeed();
proto native bool IsChangingStance();
proto native bool IsOnBack();
proto native bool IsInRoll();
proto native bool IsLeavingUncon();
proto native bool IsStandingFromBack();
proto native void StartMeleeEvade();
proto native void StartMeleeEvadeA(float pDirAngle);
proto native bool IsMeleeEvade();
proto native void SetMeleeBlock(bool pBlock);
proto native void ForceStance(int pStanceIdx);
proto native void ForceStanceUp(int pStanceIdx);
proto native void SetRunSprintFilterModifier(float value);
proto native void SetDirectionFilterModifier(float value);
proto native void SetDirectionSprintFilterModifier(float value);
proto native void SetTurnSpanModifier(float value);
proto native void SetTurnSpanSprintModifier(float value);
proto native void SetCurrentWaterLevel(float value);
}
// *************************************************************************************
// ! HumanCommandMelee - actual command melee running on
// *************************************************************************************
class HumanCommandMelee
{
private void HumanCommandMelee() {}
private void ~HumanCommandMelee() {}
proto native void ContinueCombo();
proto native bool IsInComboRange();
proto native bool WasHit();
proto native void Cancel();
proto native bool IsOnBack();
}
// *************************************************************************************
// ! HumanCommandMelee2 - actual command melee running on - Heavy/Light version
// *************************************************************************************
class HumanCommandMelee2
{
private void HumanCommandMelee2() {}
private void ~HumanCommandMelee2() {}
static const int HIT_TYPE_LIGHT = 0;
static const int HIT_TYPE_HEAVY = 1;
static const int HIT_TYPE_FINISHER = 2; //liver stab
static const int HIT_TYPE_FINISHER_NECK = 3;
proto native void ContinueCombo(bool pHeavyHit, float pComboValue, EntityAI target = null, vector hitPos = vector.Zero);
proto native bool IsInComboRange();
proto native bool WasHit();
proto native void Cancel();
proto native bool IsOnBack();
proto native int GetComboCount();
proto native int GetCurrentHitType();
proto native bool IsFinisher();
}
// *************************************************************************************
// ! HumanCommandFall - actually falling
// *************************************************************************************
class HumanCommandFall
{
private void HumanCommandFall() {}
private void ~HumanCommandFall() {}
static const int LANDTYPE_NONE = 0;
static const int LANDTYPE_LIGHT = 1;
static const int LANDTYPE_MEDIUM = 2;
static const int LANDTYPE_HEAVY = 3;
proto native bool PhysicsLanded();
proto native void Land(int pLandType);
proto native bool IsLanding();
}
// *************************************************************************************
// ! HumanCommandDeathCallback - called as action callback
// *************************************************************************************
class HumanCommandDeathCallback
{
//----------------------------------
// callbacks
void OnSimulationEnd() {};
bool ShouldSimulationBeDisabled() { return true; };
}
// *************************************************************************************
// ! HumanCommandDeath - actually dying
// *************************************************************************************
class HumanCommandDeath
{
private void HumanCommandDeath() {}
private void ~HumanCommandDeath() {}
}
// *************************************************************************************
// ! HumanCommandUnconscious - actually not dying
// *************************************************************************************
class HumanCommandUnconscious
{
private void HumanCommandUnconscious() {}
private void ~HumanCommandUnconscious() {}
proto native void WakeUp(int targetStance = -1);
proto native bool IsWakingUp();
proto native bool IsOnLand();
proto native bool IsInWater();
}
// *************************************************************************************
// ! HumanCommandDamage - additional damage
// *************************************************************************************
class HumanCommandDamage
{
private void HumanCommandDamage() {}
private void ~HumanCommandDamage() {}
}
// *************************************************************************************
// ! HumanCommandLadder ladder
// *************************************************************************************
class HumanCommandLadder
{
private void HumanCommandLadder() {}
private void ~HumanCommandLadder() {}
proto native bool CanExit();
proto native void Exit();
proto native static bool DebugDrawLadder(Building pBuilding, int pLadderIndex);
proto native static int DebugGetLadderIndex(string pComponentName);
proto native vector GetLogoutPosition();
}
// *************************************************************************************
// ! HumanCommandLadder ladder
// *************************************************************************************
class HumanCommandSwim
{
private void HumanCommandSwim() {}
private void ~HumanCommandSwim() {}
proto native void StopSwimming();
proto native static vector WaterLevelCheck(Human pHuman, vector pPosition);
}
// *************************************************************************************
// ! HumanCommandVehicle vehicle
// *************************************************************************************
class HumanCommandVehicle
{
private void HumanCommandVehicle() {}
private void ~HumanCommandVehicle() {}
proto native Transport GetTransport();
proto native int GetVehicleClass();
proto native int GetVehicleSeat();
proto native void SetVehicleType(int pVehicleType);
proto native int GetVehicleType();
proto native void GetOutVehicle();
proto native void KnockedOutVehicle();
proto native bool ShouldBeKnockedOut();
proto native void JumpOutVehicle();
proto native void SwitchSeat(int pTransportPositionIndex, int pVehicleSeat);
proto native bool IsGettingIn();
proto native bool IsGettingOut();
proto native bool IsSwitchSeat();
proto native bool WasGearChange();
proto native void SetClutchState(bool pState);
proto native void KeepInVehicleSpaceAfterLeave(bool pState);
proto native void ProcessLeaveEvents();
bool IsObjectIgnoredOnGettingOut(IEntity entity)
{
Object object;
if (!Class.CastTo(object, entity))
{
return true;
}
Transport transport = GetTransport();
if (!transport)
{
return true;
}
if (object == transport || object.GetParent() == transport)
{
return true;
}
return transport.IsIgnoredObject(object);
}
}
// *************************************************************************************
// ! HumanCommandClimb
// *************************************************************************************
class SHumanCommandClimbResult
{
bool m_bIsClimb;
bool m_bIsClimbOver;
bool m_bFinishWithFall;
bool m_bHasParent;
float m_fClimbHeight;
vector m_ClimbGrabPoint;
vector m_ClimbGrabPointNormal;
vector m_ClimbStandPoint;
vector m_ClimbOverStandPoint;
IEntity m_GrabPointParent;
IEntity m_ClimbStandPointParent;
IEntity m_ClimbOverStandPointParent;
};
enum ClimbStates
{
STATE_MOVE,
STATE_TAKEOFF,
STATE_ONTOP,
STATE_FALLING,
STATE_FINISH
};
class HumanCommandClimb
{
private void HumanCommandClimb() {}
private void ~HumanCommandClimb() {}
proto native int GetState();
proto native vector GetGrabPointWS();
proto native vector GetClimbOverStandPointWS();
proto native static bool DoClimbTest(Human pHuman, SHumanCommandClimbResult pResult, int pDebugDrawLevel);
proto native static bool DoPerformClimbTest(Human pHuman, SHumanCommandClimbResult pResult, int pDebugDrawLevel);
proto native static bool DebugDrawClimb(Human pHuman, int pLevel);
}
// *************************************************************************************
// ! HumanCommandFullBodyDamage - full body damages
// *************************************************************************************
class HumanCommandFullBodyDamage
{
private void HumanCommandFullBodyDamage() {}
private void ~HumanCommandFullBodyDamage() {}
}
enum WeaponActions
{
INTERRUPT = 15,
NONE = -1,
RELOAD = 0,
MECHANISM = 1,
CHAMBERING = 2,
CHAMBERING_LOADER = 3,
UNJAMMING = 4,
FIRE = 5,
HIDE = 6,
SHOW = 7,
RELOAD_CLIP = 8,
};
enum WeaponActionReloadTypes
{
RELOADRIFLE_MAGAZINE_BULLET = 0, // CMD_Reload_Magazine
RELOADRIFLE_NOMAGAZINE_BULLET = 1,
RELOADRIFLE_MAGAZINE_NOBULLET = 2,
RELOADRIFLE_NOMAGAZINE_NOBULLET = 3,
RELOADRIFLE_MAGAZINE_NOBULLET_OPEN = 4,
RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN = 5,
// reload action types - pistols
RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_UNCOCKED = 8, // CMD_Reload_Magazine
RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_UNCOCKED = 9,
RELOADPISTOL_MAGAZINE_BULLET_CLOSED = 10,
RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED = 11,
RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_COCKED = 12,
RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_COCKED = 13,
RELOADPISTOL_NOMAGAZINE_NOBULLET_OPENED = 14,
RELOADPISTOL_MAGAZINE_NOBULLET_OPENED = 15,
RELOAD_MAGAZINE_DETACH = 16,
RELOADRIFLE_MAGAZINE_DETACH = 17, // CMD_Reload_Magazine
RELOADSRIFLE_MAGAZINE_BULLET = 20, // CMD_Reload_Magazine
RELOADSRIFLE_NOMAGAZINE_BULLET = 21,
RELOADSRIFLE_MAGAZINE_NOBULLET = 22,
RELOADSRIFLE_NOMAGAZINE_NOBULLET = 23,
};
enum WeaponActionReloadClipTypes
{
RELOADRIFLE_CLIP_NOBULLET = 0,
RELOADRIFLE_CLIP_BULLET = 1,
}
enum WeaponActionMechanismTypes
{
MECHANISM_OPENED = 0, // CMD_Reload_BoltAction
MECHANISM_CLOSED = 1,
MECHANISM_SPECIAL = 2, // crossbow, magnum ... special mechanism action apropriate for custom weapons
MECHANISM_CLOSED_UNCOCKED = 3,
};
enum WeaponActionChamberingTypes
{
CHAMBERING_END = -1,
CHAMBERING_ONEBULLET_OPENED = 0, // CMD_Reload_Chambering
CHAMBERING_ONEBULLET_CLOSED = 1, //
CHAMBERING_ONEBULLET_CLOSED_UNCOCKED = 2,
CHAMBERING_ONEBULLET_UNIQUE_OPENED = 3, //
CHAMBERING_ONEBULLET_UNIQUE_CLOSED = 4, //
CHAMBERING_TWOBULLETS_START = 6, // plays one bullet, then second, then ends, when CHAMBERING_TWOBULLETS_END arise, it&#39;s canceled
CHAMBERING_TWOBULLETS_END = 7, // - one bullet reload with closed mechanism
CHAMBERING_STARTLOOPABLE_CLOSED_EXTRA = 9,
CHAMBERING_STARTLOOPABLE_CLOSED = 10, // start loop chambering
CHAMBERING_ENDLOOPABLE = 11, // end loop chambering
CHAMBERING_STARTLOOPABLE_CLOSED_KEEP = 12, // start loop chambering and keep last bullet
CHAMBERING_STARTLOOPABLE_OPENED = 13, //
CHAMBERING_STARTLOOPABLE_CLOSED_WITHCHAMBER = 14,
CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED = 15,
CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED = 16,
CHAMBERING_DOUBLE_1 = 17,
CHAMBERING_DOUBLE_2 = 18,
CHAMBERING_DOUBLE_3 = 19,
CHAMBERING_DOUBLE_4 = 20,
CHAMBERING_CROSSBOW_OPENED = 21, // chambering crossbow
CHAMBERING_CROSSBOW_CLOSED = 22, // chambering crossbow
CHAMBERING_CROSSBOW_FULL = 23, // chambering crossbow
CHAMBERING_DOUBLE_FIREOUT_EJECT = 24, // For manual ejecting during reload
};
enum WeaponActionChamberingLoaderTypes
{
CHAMBERINGLOADER_OPENED = 0, // CMD_Reload_ChamberingFast - one bullet reload with open mechanism
CHAMBERINGLOADER_CLOSED = 1, // - one bullet reload with closed mechanism
};
enum WeaponActionUnjammingTypes
{
UNJAMMING_START = 1, // CMD_Weapon_Jam - 0
UNJAMMING_END = 0, // 1
UNJAMMING_INTERRUPT = -1,
};
enum WeaponActionFireTypes
{
FIRE_NORMAL = 0,
FIRE_LAST = 1,
FIRE_COCKED = 2,
FIRE_UNCOCKED = 3,
FIRE_DRY = 4,
FIRE_JAM = 5
};
enum WeaponHideShowTypes
{
HIDESHOW_SLOT_2HDLEFTBACK = 0,
HIDESHOW_SLOT_RFLLEFTBACK = 1,
HIDESHOW_SLOT_1HDLEFTBACK = 2,
HIDESHOW_SLOT_2HDRIGHTBACK = 3,
HIDESHOW_SLOT_RFLRIGHTBACK = 4,
HIDESHOW_SLOT_1HDRIGHTBACK = 5,
HIDESHOW_SLOT_PISTOLBELT = 6,
HIDESHOW_SLOT_PISTOLCHEST = 7,
HIDESHOW_SLOT_KNIFEBACK = 8,
HIDESHOW_SLOT_INVENTORY = 9,
};
string WeaponActionTypeToString (int A, int AT)
{
switch (A)
{
case WeaponActions.INTERRUPT: return "Weapon interrupt";
case WeaponActions.NONE: return "---";
case WeaponActions.RELOAD: return typename.EnumToString(WeaponActionReloadTypes, AT);
case WeaponActions.MECHANISM: return typename.EnumToString(WeaponActionMechanismTypes, AT);
case WeaponActions.CHAMBERING: return typename.EnumToString(WeaponActionChamberingTypes, AT);
case WeaponActions.CHAMBERING_LOADER: return typename.EnumToString(WeaponActionChamberingLoaderTypes, AT);
case WeaponActions.UNJAMMING: return typename.EnumToString(WeaponActionUnjammingTypes, AT);
case WeaponActions.FIRE: return typename.EnumToString(WeaponActionFireTypes, AT);
case WeaponActions.HIDE: return typename.EnumToString(WeaponHideShowTypes, AT);
case WeaponActions.SHOW: return typename.EnumToString(WeaponHideShowTypes, AT);
}
return "---";
}
enum WeaponEvents
{
ATTACHMENT_HIDE,
ATTACHMENT_SHOW,
BULLET_EJECT,
BULLET_HIDE,
BULLET_HIDE2,
BULLET_IN_CHAMBER,
BULLET_IN_MAGAZINE,
BULLET_SHOW,
BULLET_SHOW2,
CANUNJAM_END,
CANUNJAM_START,
COCKED,
MAGAZINE_ATTACHED,
MAGAZINE_DETACHED,
MAGAZINE_HIDE,
MAGAZINE_SHOW,
SLIDER_OPEN,
UNJAMMED,
HAMMER_UNCOCKED,
HAMMER_COCKED,
CHANGE_HIDE,
CHANGE_SHOW,
CYLINDER_ROTATE,
};
// *************************************************************************************
// ! HumanCommandWeapons - weapon handling
// *************************************************************************************
class HumanCommandWeapons
{
private void HumanCommandWeapons() {}
private void ~HumanCommandWeapons() {}
proto native bool IsActionFinished();
proto native int GetRunningAction();
proto native int GetRunningActionType();
proto native void SetActionProgressParams(float pStart, float pEnd);
proto native bool StartAction(WeaponActions pAction, int pActionType);
proto native void RegisterEvent(string pName, int pId);
proto native int IsEvent();
proto native bool IsInWeaponReloadBulletSwitchState();
proto native void SetADS(bool pState);
proto native void LiftWeapon(bool pState);
proto native bool IsWeaponLifted();
proto native void ObstructWeapon(float pState01);
proto native float GetWeaponObstruction();
proto native float GetAimingHandsOffsetUD();
proto native float GetAimingHandsOffsetLR();
void RegisterDefaultEvents()
{
RegisterEvent("Weapon_AttachmentHide", WeaponEvents.ATTACHMENT_HIDE);
RegisterEvent("Weapon_AttachmentShow", WeaponEvents.ATTACHMENT_SHOW);
RegisterEvent("Weapon_BulletEject", WeaponEvents.BULLET_EJECT);
RegisterEvent("Weapon_BulletHide", WeaponEvents.BULLET_HIDE);
RegisterEvent("Weapon_BulletHide2", WeaponEvents.BULLET_HIDE2);
RegisterEvent("Weapon_BulletInChamber", WeaponEvents.BULLET_IN_CHAMBER);
RegisterEvent("Weapon_BulletInMagazine", WeaponEvents.BULLET_IN_MAGAZINE);
RegisterEvent("Weapon_BulletShow", WeaponEvents.BULLET_SHOW);
RegisterEvent("Weapon_BulletShow2", WeaponEvents.BULLET_SHOW2);
RegisterEvent("Weapon_CanUnjam_End", WeaponEvents.CANUNJAM_END);
RegisterEvent("Weapon_CanUnjam_Start", WeaponEvents.CANUNJAM_START);
RegisterEvent("Weapon_Cocked", WeaponEvents.COCKED);
RegisterEvent("Weapon_MagazineAttached", WeaponEvents.MAGAZINE_ATTACHED);
RegisterEvent("Weapon_MagazineDetached", WeaponEvents.MAGAZINE_DETACHED);
RegisterEvent("Weapon_MagazineHide", WeaponEvents.MAGAZINE_HIDE);
RegisterEvent("Weapon_MagazineShow", WeaponEvents.MAGAZINE_SHOW);
RegisterEvent("Weapon_SliderOpen", WeaponEvents.SLIDER_OPEN);
RegisterEvent("Weapon_Unjammed", WeaponEvents.UNJAMMED);
RegisterEvent("Weapon_Hammer_Uncocked", WeaponEvents.HAMMER_UNCOCKED);
RegisterEvent("Weapon_Hammer_Cocked", WeaponEvents.HAMMER_COCKED);
RegisterEvent("Weapon_Change_Hide", WeaponEvents.CHANGE_HIDE);
RegisterEvent("Weapon_Change_Show", WeaponEvents.CHANGE_SHOW);
RegisterEvent("Weapon_CylinderRotate", WeaponEvents.CYLINDER_ROTATE);
}
//----------------------------------------------------
// weapon init states
proto native void SetInitState(int pFrameIndex);
proto native static void StaticSetInitState(Human pHuman, int pFrameIdx);
//----------------------------------------------------
//
proto native float GetBaseAimingAngleUD();
proto native float GetBaseAimingAngleLR();
//----------------------------------------------------
// throwing
proto native void SetThrowingMode(bool pState);
proto native bool IsThrowingMode();
proto native void ThrowItem(int throwType);
proto native bool WasItemLeaveHandsEvent();
//----------------------------------------------------
// debug copy
proto native int DebugIsEvent();
proto native void DebugResetEvents();
}
// *************************************************************************************
// ! HumanCommandAdditives - additional damage
// *************************************************************************************
class HumanCommandAdditives
{
private void HumanCommandAdditives() {}
private void ~HumanCommandAdditives() {}
proto native void SetInjured(float pValue, bool pInterpolate);
proto native void SetExhaustion(float pValue, bool pInterpolate);
proto native void SetTalking(bool pValue);
proto native void StartModifier(int pType);
proto native void CancelModifier();
proto native bool IsModifierActive();
}
// *************************************************************************************
// ! HumanMovementState - movement state, command, stance, movement, human.GetMovementState(movementState)
// *************************************************************************************
class HumanMovementState
{
int m_CommandTypeId;
int m_iStanceIdx;
int m_iMovement;
float m_fLeaning;
int m_LocalMovement = -1;
bool IsRaised()
{
return m_iStanceIdx >= DayZPlayerConstants.STANCEIDX_RAISEDERECT;
}
bool IsRaisedInProne()
{
return m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE;
}
bool IsInProne()
{
return m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE;
}
bool IsInRaisedProne()
{
return m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE;
}
bool IsLeaning()
{
return m_fLeaning != 0;
}
}
enum HumanRelativeHeadingMode
{
CURRENT,
INPUT,
};
class HumanCommandScript
{
// HumanCommandScript(Human pHuman);
void OnActivate() { };
void OnDeactivate() { };
//---------------------------------------------------------------
// usable everywhere
proto native void SetFlagFinished(bool pFinished);
#ifdef FEATURE_NETWORK_RECONCILIATION
proto native void OverrideCorrectionType(AnimPhysCorrectionType correctionType);
#endif
proto native void SetHeading(float yawAngle, float filterDt = -1, float maxYawSpeed = FLT_MAX);
proto native void AddHeadingRelativeTo(HumanRelativeHeadingMode mode, float yawAngle, float filterDt = -1, float maxYawSpeed = FLT_MAX);
int GetCurrentStance()
{
return DayZPlayerConstants.STANCEIDX_ERECT;
}
int GetCurrentMovement()
{
return DayZPlayerConstants.MOVEMENT_IDLE;
}
float GetCurrentLeaning()
{
return 0.0;
}
//---------------------------------------------------------------
// PreAnim Update
void PreAnimUpdate(float pDt);
proto native void PreAnim_CallCommand(int pCommand, int pParamInt, float pParamFloat);
proto native void PreAnim_SetFloat(int pVar, float pFlt);
proto native void PreAnim_SetInt(int pVar, int pInt);
proto native void PreAnim_SetBool(int pVar, bool pBool);
void PreAnim_SetFilteredHeading(float pYawAngle, float pFilterDt, float pMaxYawSpeed)
{
SetHeading(pYawAngle, pFilterDt, pMaxYawSpeed);
}
//---------------------------------------------------------------
// PrePhys Update
void PrePhysUpdate(float pDt);
proto native bool PrePhys_IsEvent(int pEvent);
proto native bool PrePhys_IsTag(int pTag);
proto native bool PrePhys_GetTranslation(out vector pOutTransl); // vec3 in local space !
proto native bool PrePhys_GetRotation(out float pOutRot[4]); // quaternion in local space !
proto native void PrePhys_SetTranslation(vector pInTransl); // vec3 in local space !
proto native void PrePhys_SetRotation(float pInRot[4]); // quaternion in local space !
//---------------------------------------------------------------
// PostPhys Update
bool PostPhysUpdate(float pDt);
proto native void PostPhys_GetPosition(out vector pOutTransl);
proto native void PostPhys_GetRotation(out float pOutRot[4]);
proto native void PostPhys_SetPosition(vector pInTransl);
proto native void PostPhys_SetRotation(float pInRot[4]);
proto native void PostPhys_LockRotation();
}
enum HumanMoveCommandID
{
None = 0,
CommandMove = 1,
CommandMelee = 2,
CommandMelee2 = 4,
CommandFall = 8,
CommandClimb = 16,
CommandDeath = 32,
CommandUnconscious = 64,
CommandDamageFullbody = 128,
CommandDamageAdditive = 256,
CommandLadder = 512,
CommandSwim = 1024,
CommandVehicle = 2048,
CommandActionFullbody = 4096,
CommandActionAdditive = 8192
}
class HumanType : ManType
{
};
// *************************************************************************************
// ! Human - human script interface
// *************************************************************************************
class Human : Man
{
proto native void GetTransformWS(out vector pTm[4]);
proto native void PhysicsGetTransformWS(out vector pTm[4]);
proto native void PhysicsGetTransformLS(out vector pTm[4]);
proto native vector PhysicsGetPositionWS();
proto native vector PhysicsGetPositionLS();
proto native bool CheckFreeSpace(vector localDir, float distance, bool useHeading, vector posOffset = vector.Zero, float xzScale = 1.0);
proto float CollisionMoveTest(vector dir, vector offset, float xzScale, IEntity ignoreEntity, out IEntity hitEntity, out vector hitPosition, out vector hitNormal);
//---------------------------------------------------------
// link/unlink to/from local space (enfusion hierachy)
proto native void LinkToLocalSpaceOf(notnull IEntity child, vector pLocalSpaceMatrix[4]);
proto native void UnlinkFromLocalSpace();
//---------------------------------------------------------
// heading component
proto native void AlignPositionWS(vector position);
proto native void AlignTranslationWS(vector translation);
proto native void AlignTranslationLS(vector translation);
proto native void AlignDirectionWS(vector direction);
void OnPhysMove(float dt, vector transform[4]) {}
//---------------------------------------------------------
// bone transforms
proto native int GetBoneIndexByName(string pBoneName);
proto native HumanAnimInterface GetAnimInterface();
//---------------------------------------------------------
// physic props
proto native bool PhysicsIsFalling(bool pValidate);
proto native IEntity PhysicsGetFloorEntity();
proto native IEntity PhysicsGetLinkedEntity();
proto native bool PhysicsWasSlidingOffLinkedEntity();
proto native void PhysicsGetVelocity(out vector pVelocity);
proto native void PhysicsEnableGravity(bool pEnable);
proto native bool PhysicsIsSolid();
proto native void PhysicsSetSolid(bool pSolid);
proto native void PhysicsSetRagdoll(bool pEnable);
//---------------------------------------------------------
// controller
proto native HumanInputController GetInputController();
//---------------------------------------------------------
// item accessor
proto native HumanItemAccessor GetItemAccessor();
//---------------------------------------------------------
// commands
proto native void GetMovementState(HumanMovementState pState);
proto native int GetCurrentCommandID();
proto native HumanCommandMove StartCommand_Move();
proto native HumanCommandMove GetCommand_Move();
proto native HumanCommandMelee StartCommand_Melee(EntityAI pTarget);
proto native HumanCommandMelee GetCommand_Melee();
proto native HumanCommandMelee2 StartCommand_Melee2(EntityAI pTarget, int pHitType, float pComboValue, vector hitPos = vector.Zero);
proto native HumanCommandMelee2 GetCommand_Melee2();
proto native HumanCommandFall StartCommand_Fall(float pYVelocity);
proto native HumanCommandFall GetCommand_Fall();
proto native HumanCommandLadder StartCommand_Ladder(Building pBuilding, int pLadderIndex);
proto native HumanCommandLadder GetCommand_Ladder();
proto native HumanCommandSwim StartCommand_Swim();
proto native HumanCommandSwim GetCommand_Swim();
proto native HumanCommandVehicle StartCommand_Vehicle(Transport pTransport, int pTransportPositionIndex, int pVehicleSeat, bool fromUnconscious = false);
proto native HumanCommandVehicle GetCommand_Vehicle();
proto native HumanCommandClimb StartCommand_Climb(SHumanCommandClimbResult pClimbResult, int pType);
proto native HumanCommandClimb GetCommand_Climb();
proto native HumanCommandDeathCallback StartCommand_Death(int pType, float pDirection, typename pCallbackClass, bool pKeepInLocalSpaceAfterLeave = false);
proto native HumanCommandDeathCallback GetCommand_Death();
proto native HumanCommandUnconscious StartCommand_Unconscious(float pType);
proto native HumanCommandUnconscious GetCommand_Unconscious();
// -> of type 0,...
// -> from direction (lback -180,left -90,0,righ 90,rightback 180)
proto native HumanCommandFullBodyDamage StartCommand_Damage(int pType, float pDirection);
proto native HumanCommandFullBodyDamage GetCommand_Damage();
proto native HumanCommandActionCallback StartCommand_Action(int pActionID, typename pCallbackClass, int pStanceMask);
proto native HumanCommandActionCallback GetCommand_Action();
//---------------------------------------------------------
// command modifiers
proto native int GetCommandModifierCount();
proto native int GetCommandModifierID(int pIndex);
proto native HumanCommandAdditives GetCommandModifier_Additives();
proto native HumanCommandWeapons GetCommandModifier_Weapons();
// --- modifier/additive actions - played on COMMANDID_MOVE command
proto native HumanCommandActionCallback AddCommandModifier_Action(int pActionID, typename pCallbackClass);
proto native void DeleteCommandModifier_Action(HumanCommandActionCallback pCallback);
proto native HumanCommandActionCallback GetCommandModifier_Action();
// -> of type 0,...
// -> from direction (lback -180,left -90,0,righ 90,rightback 180)
proto native HumanCommandDamage AddCommandModifier_Damage(int pType, float pDirection);
proto native void DeleteCommandModifier_Damage(HumanCommandDamage pDamage);
proto native HumanCommandDamage GetCommandModifier_Damage();
proto native HumanCommandScript StartCommand_Script(HumanCommandScript pHumanCommand);
proto native HumanCommandScript StartCommand_ScriptInst(typename pCallbackClass);
proto native HumanCommandScript GetCommand_Script();
//---------------------------------------------------------
// debug info for Item Accessor
proto native owned string DebugGetItemClass();
proto native owned string DebugGetItemSuperClass();
proto native owned string DebugGetItemAnimInstance();
//---------------------------------------------------------
// helper functions for disabling simulation upon death
proto native void StartDeath();
proto native void ResetDeath();
proto native void ResetDeathCooldown();
proto native bool IsDeathProcessed();
proto native bool IsDeathConditionMet();
//--------------------------------------------------------
// commands start/finish events
void OnCommandMoveStart();
void OnCommandMoveFinish();
void OnCommandMeleeStart();
void OnCommandMeleeFinish();
void OnCommandMelee2Start();
void OnCommandMelee2Finish();
void OnCommandFallStart();
void OnCommandFallFinish();
void OnCommandClimbStart();
void OnCommandClimbFinish();
void OnCommandDeathStart();
void OnCommandDeathFinish();
void OnCommandUnconsciousStart();
void OnCommandUnconsciousFinish();
void OnCommandDamageFullbodyStart();
void OnCommandDamageFullbodyFinish();
void OnCommandDamageAdditiveStart();
void OnCommandDamageAdditiveFinish();
void OnCommandLadderStart();
void OnCommandLadderFinish();
void OnCommandSwimStart();
void OnCommandSwimFinish();
void OnCommandVehicleStart();
void OnCommandVehicleFinish();
void OnCommandActionFullbodyStart();
void OnCommandActionFullbodyFinish();
void OnCommandActionAdditiveStart();
void OnCommandActionAdditiveFinish();
void OnStanceChange(int previousStance, int newStance);
bool CanChangeStance(int previousStance, int newStance);
//--------------------------------------------------------
// rolling API
bool CanRoll();
void OnRollStart(bool isToTheRight);
void OnRollFinish();
//--------------------------------------------------------
// vehicle API
void OnVehicleSeatDriverEnter();
void OnVehicleSeatDriverLeft();
proto native bool IsControllingVehicle();
}