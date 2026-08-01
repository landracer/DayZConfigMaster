// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/barrel_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Barrel_ColorBase : DeployableContainer_Base
{
    private bool m_IsLocked = false;
    private ref Timer m_BarrelOpener;
 
    protected ref RainProcurementComponentBarrel m_RainProcComponent; 
    protected ref OpenableBehaviour m_Openable;
    
    override void InitItemVariables()
    {
        super.InitItemVariables();
        
        m_BarrelOpener = new Timer();
 
        m_Openable = new OpenableBehaviour(false);
        m_RainProcurement = new RainProcurementManager(this); //legacy reasons
        m_RainProcComponent = new RainProcurementComponentBarrel(this);
        
        m_HalfExtents = Vector(0.30,0.85,0.30);
        
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
