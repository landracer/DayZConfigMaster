// ======================================================================
// File: zombiebase_8c_source.html
// Category: other
// ======================================================================

class ZombieBase extends DayZInfected
{
const float TARGET_CONE_ANGLE_CHASE = 20;
const float TARGET_CONE_ANGLE_FIGHT = 30;
const float ORIENTATION_SYNC_THRESHOLD = 30; //threshold for local heading/orientation sync
const float SHOCK_TO_STUN_MULTIPLIER = 2.82;
protected int m_StanceVariation = 0;
protected int m_LastMindState = -1;
protected float m_LastMovementSpeed = -1;
protected bool m_KnuckleLand = false;
protected float m_KnuckleOutTimer = 0;
protected int m_MindState = -1;
protected int m_OrientationLocal = -1; //local &#39;companion&#39; value for sync checking
protected int m_OrientationSynced = -1;
protected float m_OrientationTimer;
protected float m_MovementSpeed = -1;
protected vector m_DefaultHitPosition;
protected float m_DeltaTime;
protected AbstractWave m_LastSoundVoiceAW;
protected ref InfectedSoundEventHandler m_InfectedSoundEventHandler;
protected ref array<Object> m_AllTargetObjects;
protected ref array<typename>m_TargetableObjects;
//static ref map<int,ref array<string>> m_FinisherSelectionMap; //! which selections in the FireGeometry trigger which finisher on hit (when applicable)
protected bool m_IsCrawling; //&#39;DayZInfectedCommandCrawl&#39; is transition to crawl only, &#39;DayZInfectedCommandMove&#39; used after that, hence this VARIABLE_WET
protected bool m_FinisherInProgress = false; //is this object being backstabbed?
protected ref ArrowManagerBase m_ArrowManager;
//-------------------------------------------------------------
void ZombieBase()
{
Init();
}
void Init()
{
SetEventMask(EntityEvent.INIT | EntityEvent.CONTACT);
m_IsCrawling = false;
RegisterNetSyncVariableInt("m_MindState", -1, 4);
RegisterNetSyncVariableInt("m_OrientationSynced", 0, 359);
RegisterNetSyncVariableFloat("m_MovementSpeed", -1, 3);
RegisterNetSyncVariableBool("m_IsCrawling");
m_DefaultHitPosition = SetDefaultHitPosition(GetDayZInfectedType().GetDefaultHitPositionComponent());
if ( !g_Game.IsDedicatedServer() )
{
m_LastSoundVoiceAW = null;
m_InfectedSoundEventHandler = new InfectedSoundEventHandler(this);
}
m_AllTargetObjects = new array<Object>;
m_TargetableObjects = new array<typename>;
m_TargetableObjects.Insert(PlayerBase);
m_TargetableObjects.Insert(AnimalBase);
m_OrientationTimer = 0;
m_ArrowManager = new ArrowManagerBase(this);
}
override void OnVariablesSynchronized()
{
DebugSound("[Infected @ " + this + "][OnVariablesSynchronized]");
HandleSoundEvents();
if ( m_OrientationLocal != m_OrientationSynced )
{
m_OrientationLocal = m_OrientationSynced;
}
}
//-------------------------------------------------------------
override void EOnInit(IEntity other, int extra)
{
if ( !g_Game.IsMultiplayer() || g_Game.IsServer() )
{
m_StanceVariation = Math.RandomInt(0, 4);
DayZInfectedCommandMove moveCommand = GetCommand_Move();
moveCommand.SetStanceVariation(m_StanceVariation);
}
}
override bool IsZombie()
{
return true;
}
override bool IsDanger()
{
return true;
}
override bool IsZombieMilitary()
{
return false;
}
bool IsMale()
{
return true;
}
override bool CanBeBackstabbed()
{
return true;
}
//-------------------------------------------------------------
override AnimBootsType GetBootsType()
{
return AnimBootsType.Boots;
}
override bool CanBeSkinned()
{
return false;
}
//-------------------------------------------------------------
override bool IsHealthVisible()
{
return false;
}
//-------------------------------------------------------------
override bool IsRefresherSignalingViable()
{
return false;
}
override bool IsSelfAdjustingTemperature()
{
return IsAlive();
}
override string GetHitComponentForAI()
{
return GetDayZInfectedType().GetHitComponentForAI();
}
override string GetDefaultHitComponent()
{
return GetDayZInfectedType().GetDefaultHitComponent();
}
override vector GetDefaultHitPosition()
{
return m_DefaultHitPosition;
}
protected vector SetDefaultHitPosition(string pSelection)
{
return GetSelectionPositionMS(pSelection);
}
override array<string> GetSuitableFinisherHitComponents()
{
return GetDayZInfectedType().GetSuitableFinisherHitComponents();
}
int GetMindStateSynced()
{
return m_MindState;
}
int GetOrientationSynced()
{
return m_OrientationSynced;
}
//-------------------------------------------------------------
void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
{
m_DeltaTime = pDt;
if ( ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished) )
{
return;
}
if ( pCurrentCommandID != DayZInfectedConstants.COMMANDID_DEATH )
{
if ( HandleDeath(pCurrentCommandID) )
return;
}
else if (!pCurrentCommandFinished)
{
return;
}
HandleMove(pCurrentCommandID);
HandleOrientation(pDt,pCurrentCommandID);
if (pCurrentCommandFinished)
{
DayZInfectedCommandMove moveCommand = StartCommand_Move();
moveCommand.SetStanceVariation(m_StanceVariation);
return;
}
if ( ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished) )
{
return;
}
if ( HandleCrawlTransition(pCurrentCommandID) )
{
return;
}
if ( HandleDamageHit(pCurrentCommandID) )
{
return;
}
DayZInfectedInputController inputController = GetInputController();
if ( inputController )
{
if ( HandleVault(pCurrentCommandID, inputController, pDt) )
{
return;
}
if ( HandleMindStateChange(pCurrentCommandID, inputController, pDt) )
{
return;
}
if ( FightLogic(pCurrentCommandID, inputController, pDt) )
{
return;
}
}
if ( ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished) )
{
return;
}
}
//-------------------------------------------------------------
void CommandHandlerDebug(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
{
if ( GetPluginManager() )
{
PluginDayZInfectedDebug infectedDebug = PluginDayZInfectedDebug.Cast(GetPluginManager().GetPluginByType(PluginDayZInfectedDebug));
if ( infectedDebug )
infectedDebug.CommandHandler(this);
}
}
//-------------------------------------------------------------
void HandleMove(int pCurrentCommandID)
{
DayZInfectedInputController ic = GetInputController();
m_MovementSpeed = ic.GetMovementSpeed();
if (Math.AbsFloat(m_LastMovementSpeed - m_MovementSpeed) >= 0.9 && m_LastMovementSpeed != m_MovementSpeed)
{
SetSynchDirty();
}
m_LastMovementSpeed = m_MovementSpeed;
}
//-------------------------------------------------------------
void HandleOrientation(float pDt, int pCurrentCommandID)
{
m_OrientationTimer += pDt;
int yaw = Math.Round(GetOrientation()[0]);
yaw = Math.NormalizeAngle(yaw);
//atan2(sin(x-y), cos(x-y))
float angleSourceRad = m_OrientationSynced * Math.DEG2RAD;
float angleTargetRad = yaw * Math.DEG2RAD;
float angleDiffRad = Math.Atan2(Math.Sin(angleTargetRad - angleSourceRad), Math.Cos(angleSourceRad - angleTargetRad));
angleDiffRad *= Math.RAD2DEG;
angleDiffRad = Math.Round(angleDiffRad);
if (m_OrientationTimer >= 2.0 || m_OrientationSynced == -1 || Math.AbsInt(angleDiffRad) > ORIENTATION_SYNC_THRESHOLD)
{
m_OrientationTimer = 0.0;
if (m_OrientationSynced == -1 || Math.AbsInt(angleDiffRad) > 5)
{
m_OrientationSynced = yaw;
SetSynchDirty();
}
}
}
//-------------------------------------------------------------
float m_DamageHitDirection = 0;
int m_DeathType = 0;
bool HandleDeath(int pCurrentCommandID)
{
if ( !IsAlive() || m_FinisherInProgress )
{
StartCommand_Death(m_DeathType, m_DamageHitDirection);
m_MovementSpeed = -1;
m_MindState = -1;
SetSynchDirty();
return true;
}
return false;
}
bool EvaluateDeathAnimationEx(EntityAI pSource, ZombieHitData data, out int pAnimType, out float pAnimHitDir)
{
bool ret = EvaluateDeathAnimation(pSource,data.m_DamageZone,data.m_AmmoType,pAnimType,pAnimHitDir);
return ret;
}
bool EvaluateDeathAnimation(EntityAI pSource, string pComponent, string pAmmoType, out int pAnimType, out float pAnimHitDir)
{
bool doPhxImpulse = g_Game.ConfigGetInt("cfgAmmo " + pAmmoType + " doPhxImpulse") > 0;
pAnimType = doPhxImpulse;
pAnimHitDir = ComputeHitDirectionAngle(pSource);
if ( doPhxImpulse )
{
vector impulse = 80 * m_TransportHitVelocity;
impulse[1] = 80 * 1.5;
//Print("Impulse: " + impulse.ToString());
dBodyApplyImpulse(this, impulse);
}
return true;
}
//-------------------------------------------------------------
int m_ActiveVaultType = -1;
int GetVaultType(float height)
{
if ( height <= 0.6 )
return 0;
else if ( height <= 1.1 )
return 1;
else if ( height <= 1.6 )
return 2;
else
return 3;
}
bool HandleVault(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
{
if ( pCurrentCommandID == DayZInfectedConstants.COMMANDID_VAULT )
{
DayZInfectedCommandVault vaultCmd = GetCommand_Vault();
if ( vaultCmd && vaultCmd.WasLand() )
{
m_KnuckleOutTimer = 0;
m_KnuckleLand = true;
}
if ( m_KnuckleLand )
{
m_KnuckleOutTimer += pDt;
if ( m_KnuckleOutTimer > 2.0 )
StartCommand_Vault(-1);
}
return true;
}
if ( pInputController.IsVault() )
{
float vaultHeight = pInputController.GetVaultHeight();
int vaultType = GetVaultType(vaultHeight);
m_KnuckleLand = false;
StartCommand_Vault(vaultType);
return true;
}
return false;
}
//-------------------------------------------------------------
bool HandleMindStateChange(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
{
DayZInfectedCommandMove moveCommand = GetCommand_Move();
m_MindState = pInputController.GetMindState();
if ( m_LastMindState != m_MindState )
{
switch ( m_MindState )
{
case DayZInfectedConstants.MINDSTATE_CALM:
if ( moveCommand && !moveCommand.IsTurning() )
moveCommand.SetIdleState(0);
break;
case DayZInfectedConstants.MINDSTATE_DISTURBED:
if ( moveCommand && !moveCommand.IsTurning() )
moveCommand.SetIdleState(1);
break;
case DayZInfectedConstants.MINDSTATE_CHASE:
if ( moveCommand && !moveCommand.IsTurning() && (m_LastMindState < DayZInfectedConstants.MINDSTATE_CHASE) )
moveCommand.SetIdleState(2);
break;
}
m_LastMindState = m_MindState;
m_AttackCooldownTime = 0.0;
SetSynchDirty();
}
return false;
}
//-------------------------------------------------------------
protected void HandleSoundEvents()
{
if ( !m_InfectedSoundEventHandler )
{
return;
}
if ( !IsAlive() )
{
m_InfectedSoundEventHandler.Stop();
return;
}
switch ( m_MindState )
{
case DayZInfectedConstants.MINDSTATE_CALM:
m_InfectedSoundEventHandler.PlayRequest(EInfectedSoundEventID.MINDSTATE_CALM_MOVE);
break;
case DayZInfectedConstants.MINDSTATE_ALERTED:
m_InfectedSoundEventHandler.PlayRequest(EInfectedSoundEventID.MINDSTATE_ALERTED_MOVE);
break;
case DayZInfectedConstants.MINDSTATE_DISTURBED:
m_InfectedSoundEventHandler.PlayRequest(EInfectedSoundEventID.MINDSTATE_DISTURBED_IDLE);
break
case DayZInfectedConstants.MINDSTATE_CHASE:
m_InfectedSoundEventHandler.PlayRequest(EInfectedSoundEventID.MINDSTATE_CHASE_MOVE);
break;
default:
m_InfectedSoundEventHandler.Stop();
break;
}
DebugSound("[Infected @ " + this + "][MindState]" + typename.EnumToString(DayZInfectedConstants, m_MindState));
DebugSound("[Infected @ " + this + "][SoundEventID]" + typename.EnumToString(EInfectedSoundEventID, m_InfectedSoundEventHandler.GetCurrentStateEventID()));
}
AbstractWave ProcessVoiceFX(string pSoundSetName)
{
SoundParams soundParams;
SoundObjectBuilder soundObjectBuilder;
SoundObject soundObject;
if (!g_Game.IsDedicatedServer())
{
soundParams = new SoundParams( pSoundSetName );
if ( !soundParams.IsValid() )
{
//SoundError("Invalid sound set.");
return null;
}
soundObjectBuilder = new SoundObjectBuilder( soundParams );
soundObject = soundObjectBuilder.BuildSoundObject();
AttenuateSoundIfNecessary(soundObject);
return PlaySound(soundObject, soundObjectBuilder);
}
return null;
}
override void OnSoundVoiceEvent(int event_id, string event_user_string)
{
//super.OnSoundVoiceEvent(event_id, event_user_string);
AnimSoundVoiceEvent voice_event = GetCreatureAIType().GetSoundVoiceEvent(event_id);
if (voice_event != null)
{
if (m_InfectedSoundEventHandler) // && m_InfectedSoundEventHandler.IsPlaying())
{
m_InfectedSoundEventHandler.Stop();
DebugSound("[Infected @ " + this + "][SoundEvent] InfectedSoundEventHandler - stop all");
}
if (m_LastSoundVoiceAW != null)
{
DebugSound("[Infected @ " + this + "][AnimVoiceEvent] Stopping LastAW");
m_LastSoundVoiceAW.Stop();
}
ProcessSoundVoiceEvent(voice_event, m_LastSoundVoiceAW);
HandleSoundEvents();
}
}
protected void ProcessSoundVoiceEvent(AnimSoundVoiceEvent sound_event, out AbstractWave aw)
{
if (!g_Game.IsDedicatedServer())
{
SoundObjectBuilder objectBuilder = sound_event.GetSoundBuilder();
if (NULL != objectBuilder)
{
objectBuilder.AddEnvSoundVariables(GetPosition());
SoundObject soundObject = objectBuilder.BuildSoundObject();
AttenuateSoundIfNecessary(soundObject);
aw = PlaySound(soundObject, objectBuilder);
}
}
if (g_Game.IsServer())
{
if (sound_event.m_NoiseParams != NULL)
g_Game.GetNoiseSystem().AddNoise(this, sound_event.m_NoiseParams, NoiseAIEvaluate.GetNoiseReduction(g_Game.GetWeather()));
}
}
//-------------------------------------------------------------
EntityAI m_ActualTarget = null;
float m_AttackCooldownTime = 0;
DayZInfectedAttackType m_ActualAttackType = null;
bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
{
if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_MOVE)
{
// we attack only in chase & fight state
int mindState = pInputController.GetMindState();
if (mindState == DayZInfectedConstants.MINDSTATE_CHASE)
{
return ChaseAttackLogic(pCurrentCommandID, pInputController, pDt);
}
else if (mindState == DayZInfectedConstants.MINDSTATE_FIGHT)
{
return FightAttackLogic(pCurrentCommandID, pInputController, pDt);
}
}
else if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_ATTACK)
{
DayZInfectedCommandAttack attackCommand = GetCommand_Attack();
if (attackCommand && attackCommand.WasHit())
{
if (m_ActualTarget != null)
{
if (m_ActualTarget.GetMeleeTargetType() == EMeleeTargetType.NONALIGNABLE)
return false;
bool playerInBlockStance = false;
vector targetPos = m_ActualTarget.GetPosition();
vector hitPosWS = targetPos;
vector zombiePos = GetPosition();
PlayerBase playerTarget = PlayerBase.Cast(m_ActualTarget);
if (playerTarget)
{
playerInBlockStance = playerTarget.GetMeleeFightLogic() && playerTarget.GetMeleeFightLogic().IsInBlock();
}
if (vector.DistanceSq(targetPos, zombiePos) <= m_ActualAttackType.m_Distance * m_ActualAttackType.m_Distance)
{
if (playerInBlockStance && (Math.RAD2DEG * Math.AbsFloat(Math3D.AngleFromPosition(targetPos, MiscGameplayFunctions.GetHeadingVector(playerTarget), zombiePos))) <= GameConstants.AI_MAX_BLOCKABLE_ANGLE)
{
if (m_ActualAttackType.m_IsHeavy == 1)
{
hitPosWS = m_ActualTarget.ModelToWorld(m_ActualTarget.GetDefaultHitPosition());
DamageSystem.CloseCombatDamageName(this, m_ActualTarget, m_ActualTarget.GetHitComponentForAI(), "MeleeZombie", hitPosWS);
}
else
{
hitPosWS = m_ActualTarget.ModelToWorld(m_ActualTarget.GetDefaultHitPosition());
DamageSystem.CloseCombatDamageName(this, m_ActualTarget, m_ActualTarget.GetHitComponentForAI(), "Dummy_Light", hitPosWS);
}
}
else
{
hitPosWS = m_ActualTarget.ModelToWorld(m_ActualTarget.GetDefaultHitPosition());
DamageSystem.CloseCombatDamageName(this, m_ActualTarget, m_ActualTarget.GetHitComponentForAI(), m_ActualAttackType.m_AmmoType, hitPosWS);
}
}
}
}
return true;
}
return false;
}
bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
{
// always update target - it can be destroyed
m_ActualTarget = pInputController.GetTargetEntity();
PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
if ( pb && pb.GetCommand_Vehicle() )
{
return false;
}
if ( m_ActualTarget == NULL )
return false;
vector targetPos = m_ActualTarget.GetPosition();
if ( !CanAttackToPosition(targetPos) )
return false;
float targetDist = vector.Distance(targetPos, this.GetPosition());
int pitch = GetAttackPitch(m_ActualTarget);
m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
if (m_ActualAttackType)
{
Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), TARGET_CONE_ANGLE_CHASE, m_ActualAttackType.m_Distance, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);
if (m_ActualTarget != target)
{
m_AllTargetObjects.Clear();
return false;
}
StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
return true;
}
return false;
}
bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
{
// always update target - it can be destroyed
m_ActualTarget = pInputController.GetTargetEntity();
PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
if (pb && pb.GetCommand_Vehicle())
return false;
if (m_AttackCooldownTime > 0)
{
m_AttackCooldownTime -= pDt * GameConstants.AI_ATTACKSPEED;
return false;
}
if (m_ActualTarget == null)
return false;
vector targetPos = m_ActualTarget.GetPosition();
float targetDist = vector.Distance(targetPos, this.GetPosition());
int pitch = GetAttackPitch(m_ActualTarget);
if (!CanAttackToPosition(targetPos))
return false;
m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, targetDist, pitch);
if (m_ActualAttackType)
{
Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), TARGET_CONE_ANGLE_FIGHT, m_ActualAttackType.m_Distance, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);
if (m_AllTargetObjects.Count() > 0 && m_AllTargetObjects[0] != m_ActualTarget)
{
m_AllTargetObjects.Clear();
return false;
}
StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
return true;
}
return false;
}
int GetAttackPitch(EntityAI target)
{
vector attackRefPos;
attackRefPos = target.GetDefaultHitPosition();
if ( attackRefPos != vector.Zero )
{
attackRefPos = target.ModelToWorld(attackRefPos);
}
else
{
attackRefPos = target.GetPosition();
}
// Now we have only erect stance, we need to get head position later too
float headPosY = GetPosition()[1];
headPosY += 1.8;
float diff = Math.AbsFloat(attackRefPos[1] - headPosY);
if ( diff < 0.3 )
return 0;
if ( headPosY > attackRefPos[1] )
return -1;
else
return 1;
}
//-------------------------------------------------------------
int m_CrawlTransition = -1;
bool HandleCrawlTransition(int pCurrentCommandID)
{
if ( m_CrawlTransition != -1 )
{
StartCommand_Crawl(m_CrawlTransition);
m_CrawlTransition = -1;
m_IsCrawling = true;
SetSynchDirty();
return true;
}
return pCurrentCommandID == DayZInfectedConstants.COMMANDID_CRAWL;
}
bool EvaluateCrawlTransitionAnimation(EntityAI pSource, string pComponent, string pAmmoType, out int pAnimType)
{
pAnimType = -1;
if ( pComponent == "LeftLeg" && GetHealth(pComponent, "Health") == 0 )
pAnimType = 0;
else if ( pComponent == "RightLeg" && GetHealth(pComponent, "Health") == 0 )
pAnimType = 2;
if ( pAnimType != -1 )
{
vector targetDirection = GetDirection();
vector toSourceDirection = (pSource.GetPosition() - GetPosition());
targetDirection[1] = 0;
toSourceDirection[1] = 0;
targetDirection.Normalize();
toSourceDirection.Normalize();
float cosFi = vector.Dot(targetDirection, toSourceDirection);
if ( cosFi >= 0 ) // front
pAnimType++;
}
return pAnimType != -1;
}
//-------------------------------------------------------------
bool m_DamageHitToProcess = false;
bool m_DamageHitHeavy = false;
int m_DamageHitType = 0;
float m_ShockDamage = 0;
const float HIT_INTERVAL_MIN = 0.3; // Minimum time in seconds before a COMMANDID_HIT to COMMANDID_HIT transition is allowed
float m_HitElapsedTime = HIT_INTERVAL_MIN;
bool HandleDamageHit(int pCurrentCommandID)
{
if ( pCurrentCommandID == DayZInfectedConstants.COMMANDID_HIT )
{
// Throttle hit command up to a fixed rate
if ( m_HitElapsedTime < HIT_INTERVAL_MIN )
{
m_HitElapsedTime += m_DeltaTime;
m_DamageHitToProcess = false;
m_ShockDamage = 0;
return false;
}
}
if ( m_DamageHitToProcess )
{
int randNum = Math.RandomIntInclusive(0, 100);
float stunChange = SHOCK_TO_STUN_MULTIPLIER * m_ShockDamage;
if ( m_DamageHitHeavy || randNum <= stunChange || ( m_MindState == DayZInfectedConstants.MINDSTATE_CALM || m_MindState == DayZInfectedConstants.MINDSTATE_DISTURBED ) )
{
StartCommand_Hit(m_DamageHitHeavy, m_DamageHitType, m_DamageHitDirection);
m_HitElapsedTime = 0;
}
m_DamageHitToProcess = false;
m_ShockDamage = 0;
m_HeavyHitOverride = false;
return true;
}
return false;
}
bool EvaluateDamageHitAnimation(EntityAI pSource, string pComponent, string pAmmoType, out bool pHeavyHit, out int pAnimType, out float pAnimHitDir)
{
int invertHitDir = 0; //Used to flip the heavy hit animation direction
pHeavyHit = ((g_Game.ConfigGetInt("cfgAmmo " + pAmmoType + " hitAnimation") > 0) || m_HeavyHitOverride);
invertHitDir = g_Game.ConfigGetInt("cfgAmmo " + pAmmoType + " invertHitDir");
pAnimType = 0; // belly
if ( !pHeavyHit )
{
if ( pComponent == "Torso" ) // body
pAnimType = 1;
else if ( pComponent == "Head" ) // head
pAnimType = 2;
}
//pAnimHitDir = ComputeHitDirectionAngle(pSource);
pAnimHitDir = ComputeHitDirectionAngleEx(pSource, invertHitDir);
//m_ShockDamage = g_Game.ConfigGetFloat( "CfgAmmo " + pAmmoType + " DamageApplied " + "Shock " + "damage");
return true;
}
float ComputeHitDirectionAngle(EntityAI pSource)
{
vector targetDirection = GetDirection();
vector toSourceDirection = (pSource.GetPosition() - GetPosition());
targetDirection[1] = 0;
toSourceDirection[1] = 0;
targetDirection.Normalize();
toSourceDirection.Normalize();
float cosFi = vector.Dot(targetDirection, toSourceDirection);
vector cross = targetDirection * toSourceDirection;
float dirAngle = Math.Acos(cosFi) * Math.RAD2DEG;
if ( cross[1] < 0 )
dirAngle = -dirAngle;
return dirAngle;
}
float ComputeHitDirectionAngleEx(EntityAI pSource, int invertHitDir = 0)
{
vector targetDirection = GetDirection();
vector toSourceDirection = (pSource.GetPosition() - GetPosition());
targetDirection[1] = 0;
toSourceDirection[1] = 0;
targetDirection.Normalize();
toSourceDirection.Normalize();
float cosFi = vector.Dot(targetDirection, toSourceDirection);
vector cross = targetDirection * toSourceDirection;
float dirAngle = Math.Acos(cosFi) * Math.RAD2DEG;
// We will invert direction of the hit
if ( invertHitDir > 0 )
dirAngle -= 180;
if ( cross[1] < 0 )
dirAngle = -dirAngle;
return dirAngle;
}
//-------------------------------------------------------------
override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
{
super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
m_TransportHitRegistered = false;
if ( !IsAlive() )
{
ZombieHitData data = new ZombieHitData;
data.m_Component = component;
data.m_DamageZone = dmgZone;
data.m_AmmoType = ammo;
EvaluateDeathAnimationEx(source, data, m_DeathType, m_DamageHitDirection);
}
else
{
int crawlTransitionType = -1;
if ( EvaluateCrawlTransitionAnimation(source, dmgZone, ammo, crawlTransitionType) )
{
m_CrawlTransition = crawlTransitionType;
return;
}
if ( EvaluateDamageHitAnimation(source, dmgZone, ammo, m_DamageHitHeavy, m_DamageHitType, m_DamageHitDirection) )
{
if ( dmgZone )
m_ShockDamage = damageResult.GetDamage( dmgZone, "Shock" );
m_DamageHitToProcess = true;
return;
}
}
}
override void EEHitByRemote(int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
{
super.EEHitByRemote(damageType, source, component, dmgZone, ammo, modelPos);
}
protected void DebugSound(string s)
{
//Print(s);
}
//-------------------------------------------------------------
override protected void EOnContact(IEntity other, Contact extra)
{
if ( !IsAlive() )
return;
Transport transport = Transport.Cast(other);
if ( transport )
{
if ( g_Game.IsServer() )
{
RegisterTransportHit(transport);
}
}
}
override bool CanReceiveAttachment(EntityAI attachment, int slotId)
{
if ( !IsAlive() )
{
return false;
}
return super.CanReceiveAttachment(attachment, slotId);
}
override vector GetCenter()
{
return GetBonePositionWS( GetBoneIndexByName( "spine3" ) );
}
override bool IsBeingBackstabbed()
{
return m_FinisherInProgress;
}
override void SetBeingBackstabbed(int backstabType)
{
// disable AI simulation
GetAIAgent().SetKeepInIdle(true);
// select death animation
switch (backstabType)
{
case EMeleeHitType.FINISHER_LIVERSTAB:
m_DeathType = DayZInfectedDeathAnims.ANIM_DEATH_BACKSTAB;
break;
case EMeleeHitType.FINISHER_NECKSTAB:
m_DeathType = DayZInfectedDeathAnims.ANIM_DEATH_NECKSTAB;
break;
default:
m_DeathType = DayZInfectedDeathAnims.ANIM_DEATH_DEFAULT;
}
// set flag - death command will be executed
m_FinisherInProgress = true;
}
bool IsCrawling()
{
return m_IsCrawling;
}
// called from command death when stealth attack wan&#39;t successful
void OnRecoverFromDeath()
{
// enable AI simulation again
GetAIAgent().SetKeepInIdle(false);
// reset flag
m_FinisherInProgress = false;
}
override void AddArrow(Object arrow, int componentIndex, vector closeBonePosWS, vector closeBoneRotWS)
{
CachedObjectsArrays.ARRAY_STRING.Clear();
GetActionComponentNameList(componentIndex, CachedObjectsArrays.ARRAY_STRING, "fire");
int pivot = -1;
for (int i = 0; i < CachedObjectsArrays.ARRAY_STRING.Count() && pivot == -1; i++)
{
pivot = GetBoneIndexByName(CachedObjectsArrays.ARRAY_STRING.Get(i));
}
vector parentTransMat[4];
vector arrowTransMat[4];
if (pivot == -1)
{
GetTransformWS(parentTransMat);
}
else
{
vector rotMatrix[3];
Math3D.YawPitchRollMatrix(closeBoneRotWS * Math.RAD2DEG,rotMatrix);
parentTransMat[0] = rotMatrix[0];
parentTransMat[1] = rotMatrix[1];
parentTransMat[2] = rotMatrix[2];
parentTransMat[3] = closeBonePosWS;
}
arrow.GetTransform(arrowTransMat);
Math3D.MatrixInvMultiply4(parentTransMat, arrowTransMat, arrowTransMat);
// orthogonalize matrix - parent might be skewed
Math3D.MatrixOrthogonalize4(arrowTransMat);
arrow.SetTransform(arrowTransMat);
AddChild(arrow, pivot);
}
override bool IsManagingArrows()
{
return true;
}
override ArrowManagerBase GetArrowManager()
{
return m_ArrowManager;
}
}
class ZombieHitData
{
int m_Component;
string m_DamageZone;
string m_AmmoType;
}