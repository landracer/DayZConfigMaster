// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionbuildpart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BuildPartActionReciveData : ActionReciveData
{
    string m_PartType;
}
 
class BuildPartActionData : ActionData
{
    string m_PartType;
};
 
class ActionBuildPartCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float time = SetCallbackDuration(m_ActionData.m_MainItem);
        m_ActionData.m_ActionComponent = new CAContinuousTime(time);
