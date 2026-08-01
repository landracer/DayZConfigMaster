// ======================================================================
// File: dayzplayercamera3rdperson_8c_source.html
// Category: other
// ======================================================================

// *************************************************************************************
// ! DayZPlayerCamera3rdPerson - base for 3rd person cameras
// *************************************************************************************
class DayZPlayerCamera3rdPerson extends DayZPlayerCameraBase
{
static const float CONST_UD_MIN = -85.0;
static const float CONST_UD_MAX = 85.0;
static const float CONST_LR_MIN = -160.0;
static const float CONST_LR_MAX = 160.0;
void DayZPlayerCamera3rdPerson(DayZPlayer pPlayer, HumanInputController pInput)
{
if (pInput.Camera3rdIsRightShoulder())
{
m_fCameraLRShoulder = 1.0;
}
else
{
m_fCameraLRShoulder = -1.0;
}
m_fCameraLRShoulderVel[0] = 0.0;
m_CurrentCameraRoll = 0.0;
m_WeaponSwayModifier = 1;
m_fLeanDistance = 0.3;
m_fPredictCollisionRadius = 0.5;
}
//
override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
{
super.OnActivate(pPrevCamera, pPrevCameraResult);
if (pPrevCamera)
{
vector baseAngles = pPrevCamera.GetBaseAngles();
m_fUpDownAngle = baseAngles[0];
m_fLeftRightAngle = baseAngles[1];
vector addAngles = pPrevCamera.GetAdditiveAngles();
m_fUpDownAngleAdd = addAngles[0];
m_fLeftRightAngleAdd = addAngles[1];
}
}
//
override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
{
m_pPlayer.GetMovementState(m_MovementState);
m_CurrentCameraYaw = UpdateLRAngleUnlocked(m_fLeftRightAngle, m_fLeftRightAngleAdd, CONST_LR_MIN, CONST_LR_MAX, pDt);
m_CurrentCameraPitch = UpdateUDAngleUnlocked(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
ProcessCameraShake(pDt, m_fLeftRightAngle, m_fUpDownAngleAdd);
// update l/r offsets and set it as
if (m_pInput.Camera3rdIsRightShoulder())
{
m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, 1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
}
else
{
m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, -1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
}
float shoulderDist = m_fCameraLRShoulder * m_fShoulderWidth + m_MovementState.m_fLeaning * m_fLeanDistance;
Math3D.YawPitchRollMatrix(GetCurrentOrientation(), pOutResult.m_CameraTM);
if (m_iBoneIndex != -1)
{
pOutResult.m_CameraTM[3] = m_pPlayer.GetBonePositionMS(m_iBoneIndex);
}
else
{
pOutResult.m_CameraTM[3] = "0 0 0";
}
vector msOffset = m_CameraOffsetMS;
vector lsOffset = m_CameraOffsetLS;
if (m_bShoulderInLS)
{
lsOffset[0] = lsOffset[0] + shoulderDist;
}
else
{
msOffset[0] = msOffset[0] + shoulderDist;
}
// ls offset + ms offset + shoulder width
pOutResult.m_CameraTM[3] = pOutResult.m_CameraTM[3] + pOutResult.m_CameraTM[0] * lsOffset[0] + pOutResult.m_CameraTM[1] * lsOffset[1] + pOutResult.m_CameraTM[2] * lsOffset[2] + msOffset;
pOutResult.m_fDistance = m_fDistance;
pOutResult.m_fUseHeading = 1.0;
pOutResult.m_fInsideCamera = 0.0;
pOutResult.m_fPredictCollisionRadius = m_fPredictCollisionRadius;
InitCameraOnPlayer();
StdFovUpdate(pDt, pOutResult);
UpdateCameraNV(PlayerBase.Cast(m_pPlayer));
}
override vector GetBaseAngles()
{
vector a;
a[0] = m_fUpDownAngle;
a[1] = m_fLeftRightAngle;
a[2] = 0;
return a;
}
override vector GetAdditiveAngles()
{
vector a;
a[0] = m_fUpDownAngleAdd;
a[1] = m_fLeftRightAngleAdd;
a[2] = 0;
return a;
}
override string GetCameraName()
{
return "DayZPlayerCamera3rdPerson";
}
protected int m_iBoneIndex = -1;
protected vector m_CameraOffsetMS;
protected vector m_CameraOffsetLS;
protected float m_fDistance;
protected float m_fShoulderWidth;
protected bool m_bShoulderInLS;
protected float m_fRoll;
protected float m_fLeanDistance;
protected float m_fUpDownAngle;
protected float m_fUpDownAngleAdd;
protected float m_fLeftRightAngle;
protected float m_fLeftRightAngleAdd;
protected float m_fCameraLRShoulder; // -1..1 shoulderness :)
protected float m_fCameraLRShoulderVel[1]; // 0
protected float m_fPredictCollisionRadius;
ref HumanMovementState m_MovementState = new HumanMovementState();
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected
// *************************************************************************************
class DayZPlayerCamera3rdPersonErc extends DayZPlayerCamera3rdPerson
{
void DayZPlayerCamera3rdPersonErc(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonErc");
m_fDistance = 1.6;
m_CameraOffsetMS = "0.0 1.4 0.0";
m_CameraOffsetLS = "0.0 0.3 0.0";
m_fShoulderWidth = 0.3;
m_fPredictCollisionRadius = 1.2;
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonJump - 3rd person jump
// *************************************************************************************
class DayZPlayerCamera3rdPersonJump extends DayZPlayerCamera3rdPersonErc
{
void DayZPlayerCamera3rdPersonJump(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonJump");
m_fDelay = 0.15;
m_fDamping = 0.25;
m_fJumpStartY = pPlayer.GetOrigin()[1];
m_fJumpOffset = 0;
m_fDelayTimer = 0;
}
//
override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
{
super.OnUpdate(pDt, pOutResult);
float yPos = m_pPlayer.GetOrigin()[1];
float yDiff = yPos - m_fJumpStartY;
yDiff = Math.Clamp(yDiff, 0.0, 2.0);
if( m_fDelayTimer < m_fDelay )
{
m_fDelayTimer += pDt;
m_fJumpOffset = yDiff;
}
else
{
m_fJumpOffset = Math.SmoothCD(m_fJumpOffset, -(m_fDamping * yDiff), m_jumpOffsetVelocity, 0.3, 1000, pDt);
}
float newY = pOutResult.m_CameraTM[3][1] - m_fJumpOffset;
pOutResult.m_CameraTM[3][1] = newY;
}
float m_fDelay;
float m_fDamping;
float m_fDelayTimer;
float m_fJumpStartY;
float m_fJumpOffset;
float m_jumpOffsetVelocity[1];
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonJump - 3rd person jump
// *************************************************************************************
class DayZPlayerCamera3rdPersonClimb extends DayZPlayerCamera3rdPersonErc
{
void DayZPlayerCamera3rdPersonClimb(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonClimb");
m_fDistance = 1.0;
m_CameraOffsetMS = "0.0 0.3 -0.6";
m_iBoneIndex = pPlayer.GetBoneIndexByName("Spine");
m_fPredictCollisionRadius = 0.5;
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected sprint
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcSpr extends DayZPlayerCamera3rdPersonErc
{
void DayZPlayerCamera3rdPersonErcSpr(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonErcSpr");
m_fDistance = 2.0;
m_CameraOffsetMS = "0.0 1.3 0.0";
m_CameraOffsetLS = "0.0 0.3 0.0";
m_fShoulderWidth = 0.15;
m_iPelvisBone = pPlayer.GetBoneIndexByName("Pelvis");
if (m_iPelvisBone == -1)
{
Print("DayZPlayerCamera3rdPersonErcSpr: hips bone not found");
}
m_fTime = 0.0;
}
//
override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
{
m_fTime += pDt * 5;
vector tm[4];
m_pPlayer.GetBoneTransformLS(m_iPelvisBone, tm);
float xShift = tm[1][0];
m_CurrentCameraRoll = xShift * 3.0; // 3 is just made up value i like :)
// m_CurrentCameraRoll = (sin(1.5 * m_fTime)); // + 0.3 * sin(3.6 * m_fTime) + 0.2 * sin(0.7 * m_fTime)) / 1.3 * 3;
super.OnUpdate(pDt, pOutResult);
//pOutResult.m_fFovMultiplier = 1.2;
}
protected int m_iPelvisBone;
protected float m_fTime;
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErcRaised - 3rd person erected raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcRaised extends DayZPlayerCamera3rdPersonErc
{
void DayZPlayerCamera3rdPersonErcRaised(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonErcRaised");
m_fPredictCollisionRadius = 0.2;
if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_EMPTYHANDED )
{
// no item in hands
m_fDistance = 1.2;
m_fShoulderWidth = 0.4;
m_CameraOffsetMS = "0.0 1.3 0.2";
m_CameraOffsetLS = "0.0 0.3 0.0";
}
else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_TWOHANDED )
{
// two handed
m_fDistance = 1.2;
m_fShoulderWidth = 0.4;
m_CameraOffsetMS = "0.0 1.3 0.2";
m_CameraOffsetLS = "0.0 0.3 0.0";
}
else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_ONEHANDED )
{
// one handed
m_fDistance = 1.2;
m_fShoulderWidth = 0.4;
m_CameraOffsetMS = "0.0 1.3 0.2";
m_CameraOffsetLS = "0.0 0.3 0.0";
}
else
{
// weapons in hands
m_fDistance = 0.8;
m_CameraOffsetMS = "0.0 1.3 0.2";
m_CameraOffsetLS = "0.0 0.3 0.0";
m_fShoulderWidth = 0.5;
}
}
override bool IsCamera3rdRaised()
{
return true;
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErcRaised - 3rd person erected raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcRaisedMelee extends DayZPlayerCamera3rdPersonErcRaised
{
override void HandleShoulderCameraOverride(HumanInputController pInput)
{
pInput.Override3rdIsRightShoulder(HumanInputControllerOverrideType.ENABLED, pInput.Camera3rdIsRightShoulder());
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouch - 3rd person crouch
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouch extends DayZPlayerCamera3rdPerson
{
static const float CONST_CAMERAMOVEMENTHEIGH = 0.4; // 0 idle, 0.15 walk 0.3 run/sprint
void DayZPlayerCamera3rdPersonCrouch (DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonCrouch");
m_fDistance = 1.4;
m_CameraOffsetMS = "0.0 0.7 0.1";
m_CameraOffsetLS = "0.0 0.3 0.0";
m_fShoulderWidth = 0.3;
}
//
override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
{
m_pPlayer.GetMovementState(m_MovementState);
float movement = (Limit(m_MovementState.m_iMovement, 1, 3) - 1.0) * 0.5;
movement *= CONST_CAMERAMOVEMENTHEIGH;
m_fCameraHeight = Math.SmoothCD(m_fCameraHeight, movement, m_fCameraHeightVel, 0.2, 1000, pDt);
m_CameraOffsetMS[1] = 0.7 + m_fCameraHeight;
super.OnUpdate(pDt, pOutResult);
}
protected float m_fCameraHeight; // -1..1 shoulderness :)
protected float m_fCameraHeightVel[1]; // 0
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouchRaised- 3rd person crouch raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouchRaised extends DayZPlayerCamera3rdPersonCrouch
{
void DayZPlayerCamera3rdPersonCrouchRaised(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonCrouchRaised");
m_fDistance = 0.8;
m_CameraOffsetMS = "0.0 0.8 0.1"; // being overwritten by m_CameraOffsetMS[1] from DayZPlayerCamera3rdPersonCrouch
m_CameraOffsetLS = "0.0 0.3 0.0";
m_fShoulderWidth = 0.5;
}
override bool IsCamera3rdRaised()
{
return true;
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProneRaised - 3rd person prone raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonProneBase extends DayZPlayerCamera3rdPerson
{
protected vector m_BaseOffset
protected float m_BaseOffsetVelX[1], m_BaseOffsetVelY[1], m_BaseOffsetVelZ[1];
protected float m_LookAtLRAngle;
float UpdateLRAngleLookAt(float pMin, float pMax, float pDt)
{
if (m_pInput.CameraIsFreeLook() || m_bForceFreeLook)
{
m_LookAtLRAngle += m_pInput.GetAimChange()[0] * Math.RAD2DEG;
m_LookAtLRAngle = Limit(m_LookAtLRAngle, pMin, pMax);
m_fLRAngleVel[0] = 0; // reset filter
}
else
{
// smooth value back to 0
m_LookAtLRAngle = Math.SmoothCD(m_LookAtLRAngle, 0, m_fLRAngleVel, 0.14, 1000, pDt);
// m_fLeftRightAngle = 0.9 * (1.0 - pDt);
}
return m_LookAtLRAngle;
}
override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
{
super.OnActivate(pPrevCamera, pPrevCameraResult);
if (m_iBoneIndex != -1)
{
m_BaseOffset = m_pPlayer.GetBonePositionMS(m_iBoneIndex);
}
else
{
m_BaseOffset = "0 0 0";
}
}
//
override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
{
m_CurrentCameraPitch = UpdateUDAngleUnlocked(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
float orientYaw = m_pPlayer.GetOrientation()[0];
float headYaw = m_pInput.GetHeadingAngle() * Math.RAD2DEG;
m_CurrentCameraYaw = -fixAngle_180_180(orientYaw + headYaw);
// Print("OY: " + orientYaw.ToString() + " HY: " + headYaw.ToString());
// update l/r offsets and set it as
ProcessCameraShake(pDt, m_fLeftRightAngle, m_fUpDownAngleAdd);
if (m_pInput.Camera3rdIsRightShoulder())
{
m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, 1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
}
else
{
m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, -1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
}
float shoulderDist = m_fCameraLRShoulder * m_fShoulderWidth;
Math3D.YawPitchRollMatrix(GetCurrentOrientation(), pOutResult.m_CameraTM);
if (m_iBoneIndex != -1)
{
vector bonePos = m_pPlayer.GetBonePositionMS(m_iBoneIndex);
m_BaseOffset[0] = Math.SmoothCD(m_BaseOffset[0], bonePos[0], m_BaseOffsetVelX, 0.14, 1000, pDt);
m_BaseOffset[1] = Math.SmoothCD(m_BaseOffset[1], bonePos[1], m_BaseOffsetVelY, 0.14, 1000, pDt);
m_BaseOffset[2] = Math.SmoothCD(m_BaseOffset[2], bonePos[2], m_BaseOffsetVelZ, 0.14, 1000, pDt);
}
vector msOffset = m_CameraOffsetMS;
vector lsOffset = m_CameraOffsetLS;
lsOffset[0] = lsOffset[0] + shoulderDist;
// ls offset + ms offset + shoulder width
pOutResult.m_CameraTM[3] = m_BaseOffset + pOutResult.m_CameraTM[0] * lsOffset[0] + pOutResult.m_CameraTM[1] * lsOffset[1] + pOutResult.m_CameraTM[2] * lsOffset[2] + msOffset;
m_CurrentCameraYaw = UpdateLRAngleLookAt(-180, 180, pDt);
Math3D.YawPitchRollMatrix(GetCurrentOrientation(), pOutResult.m_CameraTM);
pOutResult.m_fDistance = m_fDistance;
pOutResult.m_fUseHeading = 1.0;
pOutResult.m_fInsideCamera = 0.0;
pOutResult.m_fPositionModelSpace = 1.0;
pOutResult.m_bUpdateWhenBlendOut = false;
InitCameraOnPlayer();
StdFovUpdate(pDt, pOutResult);
UpdateCameraNV(PlayerBase.Cast(m_pPlayer));
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProne - 3rd person prone
// *************************************************************************************
class DayZPlayerCamera3rdPersonProne extends DayZPlayerCamera3rdPersonProneBase
{
void DayZPlayerCamera3rdPersonProne(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonProne");
m_iBoneIndex = pPlayer.GetBoneIndexByName ("Camera3rd_Helper");
m_fDistance = 1.0;
m_CameraOffsetMS = "0.0 0.4 0.0";
m_CameraOffsetLS = "0.0 0.2 0.0";
m_fShoulderWidth = 0.5;
}
}
// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProneRaised - 3rd person prone raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonProneRaised extends DayZPlayerCamera3rdPersonProneBase
{
void DayZPlayerCamera3rdPersonProneRaised(DayZPlayer pPlayer, HumanInputController pInput)
{
//Print("new camera: DayZPlayerCamera3rdPersonProneRaised");
m_iBoneIndex = pPlayer.GetBoneIndexByName ("Camera3rd_Helper");
m_fDistance = 1.0;
m_CameraOffsetMS = "0.0 0.4 0.0";
m_CameraOffsetLS = "0.0 0.2 0.0";
m_fShoulderWidth = 0.5;
}
override bool IsCamera3rdRaised()
{
return true;
}
}