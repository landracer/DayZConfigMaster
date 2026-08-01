// Source: F:/Games/Dayz/scripts/3_game/human.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
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

    proto native void           SetDisabled(bool pState);
    
    proto void                  GetMovement(out float pSpeed, out vector pLocalDirection);

    proto native float          GetHeadingAngle();

    proto native vector         GetAimChange();

    proto native vector         GetAimDelta(float dt);

    proto native vector         GetTracking();

    proto native bool           CameraViewChanged();

    proto native bool           CameraIsFreeLook();
    
    proto native void           ResetFreeLookToggle();

    proto native bool           CameraIsTracking();

    proto native bool           Camera3rdIsRightShoulder();
 
    //--------------------------------------------------------------

    proto native bool           IsStanceChange();

    proto native bool           IsJumpClimb();
 
 
    //--------------------------------------------------------------

    proto native bool           IsMeleeEvade();

    proto native bool           IsMeleeFastAttackModifier();

    proto native int            IsMeleeLREvade();

    proto native bool           IsMeleeWeaponAttack();
 
    //--------------------------------------------------------------

    proto native bool           WeaponWasRaiseClick();

    proto native bool           IsWeaponRaised();

    proto native bool           WeaponADS();

    proto native void           ResetADS();
    
    proto native bool           IsThrowingModeChange();

    proto native void           ResetThrowingMode();
    
    proto native bool           IsWalkToggled();
 
    //--------------------------------------------------------------
    
    proto native bool           IsUseButton();

    proto native bool           IsUseButtonDown();
    
        proto native bool           IsUseItemButton();
        proto native bool           IsUseItemButtonDown();
        
        proto native bool           IsAttackButton();
        proto native bool           IsAttackButtonDown();

    proto native bool           IsSingleUse();

    proto native bool           IsContinuousUse();

    proto native bool           IsContinuousUseStart();

    proto native bool           IsContinuousUseEnd();

    proto native bool           IsImmediateAction(); //TODO: revise, may be obsolete
 
    //--------------------------------------------------------------
    
    proto native bool           IsReloadOrMechanismSingleUse();

    proto native bool           IsReloadOrMechanismContinuousUse();

    proto native bool           IsReloadOrMechanismContinuousUseStart();

    proto native bool           IsReloadOrMechanismContinuousUseEnd();

    proto native bool           IsZoom();

    proto native bool           IsZoomToggle();

    proto native void           ResetZoomToggle();

    proto native bool           IsSightChange();

    proto native bool           IsZoomIn();

    proto native bool           IsZoomOut();

    proto native bool           IsFireModeChange();

    proto native bool           IsZeroingUp();

    proto native bool           IsZeroingDown();

    proto native bool           IsHoldBreath();

    proto native void           ResetHoldBreath();
 
    //--------------------------------------------------------------

    proto native int            IsGestureSlot();

    proto native bool           IsOtherController();
 
    //--------------------------------------------------------------

    proto native int            IsQuickBarSlot();

    proto native bool           IsQuickBarSingleUse();

    proto native bool           IsQuickBarContinuousUse();

    proto native bool           IsQuickBarContinuousUseStart();

    proto native bool           IsQuickBarContinuousUseEnd();
 
 
 
    //--------------------------------------------------------------
    // LIMITS 

    proto native void           LimitsDisableSprint(bool pDisable);

    proto native bool           LimitsIsSprintDisabled();
    
 
    //--------------------------------------------------------------
    // overrides

    proto native void           OverrideMovementSpeed(HumanInputControllerOverrideType overrideType, float value);

    proto native void           OverrideMovementAngle(HumanInputControllerOverrideType overrideType, float value);
    
    proto native void           OverrideAimChangeX(HumanInputControllerOverrideType overrideType, float value);

    proto native void           OverrideAimChangeY(HumanInputControllerOverrideType overrideType, float value);

    proto native void           OverrideMeleeEvade(HumanInputControllerOverrideType overrideType, bool value);

    proto native void           OverrideRaise(HumanInputControllerOverrideType overrideType, bool value);

    proto native void           Override3rdIsRightShoulder(HumanInputControllerOverrideType overrideType, bool value);

    proto native void           OverrideFreeLook(HumanInputControllerOverrideType overrideType, bool value);
 
 
    //--------------------------------------------------------------

    private void HumanInputController()
    {
