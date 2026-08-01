// ======================================================================
// File: humansettings_8c_source.html
// Category: other
// ======================================================================

class SHumanGlobalSettings
{
private void SHumanGlobalSettings() {}
private void ~SHumanGlobalSettings() {}
float m_fPhysMoveMaxTracedDistance;
}
class SHumanCommandMoveSettings
{
private void SHumanCommandMoveSettings() {}
private void ~SHumanCommandMoveSettings() {}
float m_fRunSpringTimeout;
float m_fRunSpringMaxChange;
float m_fDirFilterTimeout;
float m_fDirFilterSprintTimeout;
float m_fDirFilterSpeed;
float m_fMaxSprintAngle;
float m_fTurnAngle;
float m_fTurnEndUpdateNTime;
float m_fTurnNextNTime;
float m_fSlidingPoseAngle;
float m_fSlidingPoseTrackTime;
float m_fSlidingPoseRepTime;
float m_fHeadingChangeLimiterIdle;
float m_fHeadingChangeLimiterWalk;
float m_fHeadingChangeLimiterRun;
float m_fLeaningSpeed;
float m_fWaterLevelSpeedRectrictionLow;
float m_fWaterLevelSpeedRectrictionHigh;
int m_iMaxSpeedNormal_WaterLevelLow;
int m_iMaxSpeedFast_WaterLevelLow;
int m_iMaxSpeedNormal_WaterLevelHigh;
int m_iMaxSpeedFast_WaterLevelHigh;
}
class SHumanCommandSwimSettings
{
private void SHumanCommandSwimSettings() {}
private void ~SHumanCommandSwimSettings() {}
float m_fAlignIdleTimeout;
float m_fAlignIdleMaxChanged;
float m_fAlignSlowTimeout;
float m_fAlignSlowMaxChanged;
float m_fAlignFastTimeout;
float m_fAlignFastMaxChanged;
float m_fMovementSpeed;
float m_fMovementSpeedFltTime;
float m_fMovementSpeedFltMaxChange;
float m_fWaterLevelSwim;
float m_fWaterLevelIn;
float m_fWaterLevelOut;
float m_fToCrouchLevel;
float m_fToErectLevel;
}
class SHumanCommandClimbSettings
{
private void SHumanCommandClimbSettings() {}
private void ~SHumanCommandClimbSettings() {}
float m_fCharHeight;
float m_fCharWidth;
float m_fFwMinHeight;
float m_fFwMaxHeight;
float m_fFwMaxDistance;
float m_fStepFreeSpace;
float m_fStepPositionAfterEdge;
float m_fStepDownRadius;
float m_fStepMinNormal;
float m_fStepVariation;
float m_fStepMinWidth;
float m_fClimbOverMaxWidth;
float m_fClimbOverMinHeight;
float m_fBackwardsCheckDist;
};