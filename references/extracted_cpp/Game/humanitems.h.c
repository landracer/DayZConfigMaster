// Source: F:/Games/Dayz/scripts/3_game/humanitems.h Source File
// Extracted from DayZ Code Explorer by Zeroy

 
// *************************************************************************************
// ! HumanItemBehaviorCfg - per item config of stances / movements   
// *************************************************************************************
class HumanItemBehaviorCfg
{
 
    // static int   DEFMOVEMENT             = MOVEMENT_IDLE | MOVEMENT_WALK | MOVEMENT_RUN | SPRINT;

    static int IKSETTING_AIMING = 0x1;
    static int IKSETTING_RHAND  = 0x2;
    static int IKSETTING_LHAND  = 0x4;  
    
    int     m_iType;
 
    int     m_iStanceMask;                  
    int     m_StanceMovements[6];           
    int     m_StanceRotation[6];            
    int     m_IKSettings[24];               
    int     m_IKSettingsMelee[2];               
    int     m_iPerItemCameraUserData;       

    float   m_fMoveHeadingFilterSpan;       
    float   m_fMoveHeadingSprintFilterSpan; 
    float   m_fMoveHeadingProneFilterSpan;  
    float   m_fMoveHeadingFilterSpeed;      

    float   m_fMeleeEvadeHeadingFilterSpan;     
    float   m_fMeleeEvadeHeadingFilterSpeed;    
 

    bool    m_bAttackLean;                  

    bool    m_bJumpAllowed;                 
    
    private bool m_bPlaceholder;
    
 
    void    SetIK(int pStance, int pMovement, bool pAim, bool pRArm, bool pLArm)
    {
        int val = 0;
        if (pAim)   val |=  IKSETTING_AIMING;
        if (pRArm)  val |=  IKSETTING_RHAND;
        if (pLArm)  val |=  IKSETTING_LHAND;
 
        // Print(&quot;Setting ik &quot; + pStance.ToString() + &quot;,&quot; +   pMovement.ToString() );
        pStance = pStance * 4;
        pStance = pStance + pMovement;
 
        // Print(&quot;Setting ik &quot; + pStance.ToString() + &quot; to value &quot; +   val.ToString() );
        m_IKSettings[pStance] = val;
    }
 
    void    SetIKStance(int pStance, bool pAim, bool pRArm, bool pLArm)
    {
        int val = 0;
        if (pAim)   val |=  IKSETTING_AIMING;
        if (pRArm)  val |=  IKSETTING_RHAND;
        if (pLArm)  val |=  IKSETTING_LHAND;
 
        int bo = pStance * 4;
 
        for (int i = 0; i < 4; i++)
        {
            m_IKSettings[bo+i] = val;
        }
    }
 
    void    SetIKMelee(int pHitType, bool pAim, bool pRArm, bool pLArm)
    {
        int val = 0;
        if (pAim)   val |=  IKSETTING_AIMING;
        if (pRArm)  val |=  IKSETTING_RHAND;
        if (pLArm)  val |=  IKSETTING_LHAND;
 
        m_IKSettingsMelee[pHitType] = val;
    }
 
    void    SetIKAll(bool pAim, bool pRArm, bool pLArm)
    {
        int val = 0;
        if (pAim)   val |=  IKSETTING_AIMING;
        if (pRArm)  val |=  IKSETTING_RHAND;
        if (pLArm)  val |=  IKSETTING_LHAND;
 
        int i;
        for (i = 0; i < 24; i++)
        {
            m_IKSettings[i] = val;
        }
 
        for (i = 0; i < 2; i++)
        {
            m_IKSettingsMelee[i] = val;
        }
