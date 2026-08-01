// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercameravehicles.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! DayZPlayerCamera1stPersonVehicle - first person in vehicle
// *************************************************************************************
class DayZPlayerCamera1stPersonVehicle extends DayZPlayerCamera1stPerson
{
    override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.OnUpdate(pDt, pOutResult);
 
        pOutResult.m_fUseHeading        = 0.0;
        pOutResult.m_bUpdateEveryFrame  = g_Game.IsPhysicsExtrapolationEnabled();
