// Source: F:/Games/Dayz/scripts/1_core/proto/envrdevice.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE

 
enum VRDeviceType
{
    DEVICE_TYPE_OCULUS,
    DEVICE_TYPE_PS4
}
 
enum VRStatus
{
    VR_STATUS_UNKNOWN,                  //< Unknown state, probably uninitialized.
    VR_STATUS_VISIBLE,                  //< The HMD is being used for rendering.
    VR_STATUS_PRESENT,                  //< The HMD port is open.
    VR_STATUS_MOUNTED,                  //< The HMD is mounted on users head.
    VR_STATUS_DISPLAY_LOST,         //< The HMD was present and disappeared.
    VR_STATUS_SHOULD_QUIT,          //< Application requested exit.
    VR_STATUS_SHOULD_RECENTER,  //< The HMD Recenter request event was triggered. 
    VR_STATUS_TRACKED,                  //< The tracking data for the HMD are up to date.
    VR_STATUS_CALIBRATING,          //< The HMD is being calibrated.
}
 
enum VRHandEnum
{
    VR_HAND_LEFT, //< HMD controller left hand.
    VR_HAND_RIGHT //< HMD controller right hand.
}
 
enum VREye
{
    VR_EYE_LEFT, // HMD left eye.
    VR_EYE_RIGHT // HMD right eye.
}

class VRDevice : Managed
{
    proto native static VRDevice GetInstance();

    proto native VRStatus               GetStatusFlags();
    proto native owned string   GetName();
    proto native VRDeviceType GetDeviceType();
    proto native void                               RecenterHeadTracking();
    proto native vector                         GetHeadPosition();
    proto native vector                         GetHeadOrientation();
    proto native vector                         GetHandPosition(VRHand hand);
    proto native vector                         GetHandOrientation(VRHand hand);
    proto native vector                         GetEyePosition(VREye eye);
    proto native vector                         GetEyeOrientation(VREye eye);
    proto native void                               SetScale(float scale);
    proto native float                          GetScale();

    proto external void                         GetEyeFOV(VREye eye, out float leftTan, out float rightTan, out float upTan, out float downTan);
};
 
enum PSVrHeadRecenterFlagsEnum
{
    VR_RECENTER_POSITION,
    VR_RECENTER_ORIENTATION
}
 
enum PSVrBrightnessRiskEnum
{
    VR_BRIGHTNESS_RISK_LOW,
    VR_BRIGHTNESS_RISK_HIGH,
    VR_BRIGHTNESS_RISK_MAX
}
 
enum PSVrTrackingQualityEnum
{
    VR_TRACKING_QUALITY_NONE,
    VR_TRACKING_QUALITY_NOT_VISIBLE,
    VR_TRACKING_QUALITY_PARTIAL,
    VR_TRACKING_QUALITY_FULL
}
 
enum PSVrDialogStatusEnum
{
    VR_DIALOG_UNKNOWN,
    VR_DIALOG_OK,
    VR_DIALOG_CANCELED,
    VR_DIALOG_RUNNING   
}

class VRDevice_PS4 : VRDevice
{
    proto native void Set2D(bool enabled);
    proto native bool ShowHmdServiceDialog();
    proto native bool ShowHmdSetupDialog();
    proto native PSVrDialogStatusEnum GetSetupDialogStatus();
    proto native PSVrDialogStatusEnum GetServiceDialogStatus();

    proto native void SetRecenterHeadFlags(PSVrHeadRecenterFlagsEnum flags);
    proto native PSVrHeadRecenterFlagsEnum GetRecenterHeadFlags();
    proto native vector GetEyeOffset(VREye eye);
    proto native void SetMinOutputColor(int color);
    proto native void SetEyeFOV(VREye eye, float leftTan, float rightTan, float upTan, float downTan);
    proto native void ResetToDeviceFOV(VREye eye);
    proto native void SetOversamplingFactor(float factor);
    proto bool GetHmdTrackingStatus(    out PSVrBrightnessRiskEnum bRisk, 
                                                                        out PSVrTrackingQualityEnum posQuality, 
                                                                        out PSVrTrackingQualityEnum orQuality);
    proto bool GetMoveTrackingStatus(   VRHand hand, 
                                                                                        out PSVrBrightnessRiskEnum bRisk, 
                                                                                        out PSVrTrackingQualityEnum posQuality, 
                                                                                        out PSVrTrackingQualityEnum orQuality);
}               

class Oculus : VRDevice
{
    // TODO:
}
 
/*
* @}
*/
 
#endif
