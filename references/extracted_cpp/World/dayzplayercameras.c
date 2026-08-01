// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercameras.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZPlayerCameras
{
    static const int    DAYZCAMERA_1ST                      = 1;        
    static const int    DAYZCAMERA_3RD_ERC                  = 2;        
    static const int    DAYZCAMERA_3RD_ERC_SPR              = 3;        
    static const int    DAYZCAMERA_3RD_ERC_RAISED           = 4;        
    static const int    DAYZCAMERA_3RD_CRO                  = 5;        
    static const int    DAYZCAMERA_3RD_CRO_RAISED           = 6;        
    static const int    DAYZCAMERA_3RD_PRO                  = 7;        
    static const int    DAYZCAMERA_3RD_PRO_RAISED           = 8;        
    static const int    DAYZCAMERA_3RD_ERC_RAISED_MELEE     = 9;        
    static const int    DAYZCAMERA_IRONSIGHTS               = 10;       
    static const int    DAYZCAMERA_OPTICS                   = 11;       
    static const int    DAYZCAMERA_1ST_UNCONSCIOUS          = 12;       
    static const int    DAYZCAMERA_3RD_JUMP                 = 13;       
    static const int    DAYZCAMERA_3RD_CLIMB                = 14;       
    
    static const int    DAYZCAMERA_1ST_VEHICLE              = 30;       
    static const int    DAYZCAMERA_3RD_VEHICLE              = 31;       
    
    static const int    PERITEMUD_INVALID           = -1;       
    static const int    PERITEMUD_EMPTYHANDED       = 20;       
    static const int    PERITEMUD_ONEHANDED         = 25;       
    static const int    PERITEMUD_TWOHANDED         = 30;       
    
    // some times for camera changes
    static const float  TIME_CAMERACHANGE_01                = 0.1;
    static const float  TIME_CAMERACHANGE_02                = 0.2;
    static const float  TIME_CAMERACHANGE_03                = 0.3;
    
 
    static ref map<int,float>                               m_TransitionTimes = new map<int,float>;
 

    // temporary moved out of the 
    static void     RegisterCameras(DayZPlayerType pType)
    {
        pType.ResetCameraCreators();

        pType.RegisterCameraCreator(DAYZCAMERA_1ST, DayZPlayerCamera1stPerson);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC, DayZPlayerCamera3rdPersonErc);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_SPR, DayZPlayerCamera3rdPersonErcSpr);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_RAISED, DayZPlayerCamera3rdPersonErcRaised);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO, DayZPlayerCamera3rdPersonCrouch);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO_RAISED, DayZPlayerCamera3rdPersonCrouchRaised);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO, DayZPlayerCamera3rdPersonProne);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO_RAISED, DayZPlayerCamera3rdPersonProneRaised);
        pType.RegisterCameraCreator(DAYZCAMERA_IRONSIGHTS, DayZPlayerCameraIronsights);
        pType.RegisterCameraCreator(DAYZCAMERA_OPTICS, DayZPlayerCameraOptics);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_RAISED_MELEE, DayZPlayerCamera3rdPersonErcRaisedMelee);
        pType.RegisterCameraCreator(DAYZCAMERA_1ST_UNCONSCIOUS, DayZPlayerCamera1stPersonUnconscious);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_JUMP, DayZPlayerCamera3rdPersonJump);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_CLIMB, DayZPlayerCamera3rdPersonClimb);
        
        pType.RegisterCameraCreator(DAYZCAMERA_1ST_VEHICLE, DayZPlayerCamera1stPersonVehicle);
        pType.RegisterCameraCreator(DAYZCAMERA_3RD_VEHICLE, DayZPlayerCamera3rdPersonVehicle);
 
        
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_CRO, 0.4, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_PRO, 0.8, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_PRO_RAISED, 0.8, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_RAISED, 0.2, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_RAISED_MELEE, 0.2, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_3RD_ERC_SPR, 0.7, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_3RD_PRO_RAISED, 0.8, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_ERC, 0.3, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_CRO, 0.3, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_PRO, 0.5, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_ERC_RAISED, 0.3, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_JUMP, 0.1, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_JUMP, DAYZCAMERA_3RD_ERC_SPR, 0.2, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_JUMP, 0.05, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_JUMP, DAYZCAMERA_3RD_ERC, 0.3, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_CLIMB, 0.3, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_CLIMB, DAYZCAMERA_3RD_ERC, 0.3, false);

        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_ERC, 0.4, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_PRO, 0.6, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_CRO_RAISED, 0.2, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
        
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_ERC, 0.9, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_CRO, 0.7, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_PRO_RAISED, 0.2, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_CRO, 0.7, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_CRO_RAISED, 0.7, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_ERC, 0.9, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_ERC_RAISED, 0.9, false);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_ERC_RAISED_MELEE, 0.9, false);
        
        RegisterTransitionTime(DAYZCAMERA_3RD_VEHICLE, DAYZCAMERA_3RD_ERC, 0.7, false);

        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_1ST, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_01, true);
        
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
        
        RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
        
        RegisterTransitionTime(DAYZCAMERA_1ST, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);  //handheld optics only
        RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);  //handheld optics only
        RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);  //handheld optics only
        RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);  //handheld optics only
        
        //Uncon camera transitions
        RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_3RD_PRO, 1.0, false);
        RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_1ST, 0.1, false);
        RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_1ST_VEHICLE, 0.5, false);
        RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_3RD_VEHICLE, 1.0, false);
        
        pType.RegisterCameraOnChangeFnStatic(DayZPlayerCameras, &quot;OnCameraChange&quot;);
