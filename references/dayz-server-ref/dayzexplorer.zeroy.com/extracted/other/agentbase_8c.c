// ======================================================================
// File: agentbase_8c_source.html
// Category: other
// ======================================================================

class AgentBase : MessageReceiverBase
{
float m_Type = 0;
float m_Invasibility;
float m_TransferabilityIn;
float m_TransferabilityOut;
float m_Digestibility = 0.1;
int m_MaxCount = 1;
int m_AutoinfectCount = 1;
float m_AutoinfectProbability = CalculateAutoinfectProbability(0); // [0..1], 0 = zero chance, 1 = 100% chance of getting this agent once per hour
float m_TransferabilityAirOut;
float m_AntibioticsResistance = 1;
protected ref map<EMedicalDrugsType, float> m_DrugResistances = new map<EMedicalDrugsType, float>();
EStatLevels m_Potency = EStatLevels.MEDIUM;
float m_DieOffSpeed = 1;
void AgentBase()
{
InitDefaults();
Init();
}
protected void InitDefaults()
{
m_DrugResistances.Insert(EMedicalDrugsType.CHELATION, 1.0);
m_DrugResistances.Insert(EMedicalDrugsType.ANTIBIOTICS, 1.0);
}
void Init();
int GetAgentType()
{
return m_Type;
}
float GetDieOffSpeedEx(PlayerBase player)
{
return GetDieOffSpeed();
}
EStatLevels GetPotencyEx(PlayerBase player)
{
return GetPotency();
}
float GetInvasibilityEx(PlayerBase player)
{
return GetInvasibility();
}
float GetDigestibilityEx(PlayerBase player)
{
return GetDigestibility();
}
EStatLevels GetPotency()
{
return m_Potency;
}
float GetDieOffSpeed()
{
return m_DieOffSpeed;
}
float GetAntiboticsResistance()
{
return m_AntibioticsResistance;
}
float GetAntibioticsResistanceEx(PlayerBase player)
{
return GetAntiboticsResistance();
}
float GetDrugResistance(EMedicalDrugsType drugType, PlayerBase player)
{
return m_DrugResistances.Get(drugType);
}
float GetInvasibility()
{
return m_Invasibility;
}
bool GrowDuringMedicalDrugsAttack(EMedicalDrugsType drugType, PlayerBase player)
{
return true;
}
float GetDigestibility()
{
return m_Digestibility;
}
float CalculateAutoinfectProbability(float userprob)
{
return ( 1 - Math.Pow( 1 - userprob, ( 1 / 1200 ) ) );
}
bool AutoinfectCheck(float deltaT, PlayerBase player)
{
if (m_AutoinfectProbability == 0.0)
return false;
float diceThrow = Math.RandomFloat01();
if (diceThrow < m_AutoinfectProbability)
return CanAutoinfectPlayer(player);
return false;
}
bool CanAutoinfectPlayer(PlayerBase player)
{
return false;
}
float GetTransferabilityIn()
{
return m_TransferabilityIn;
}
float GetTransferabilityOut()
{
return m_TransferabilityOut;
}
float GetTransferabilityAirOut()
{
return m_TransferabilityAirOut;
}
int GetMaxCount()
{
return m_MaxCount;
}
int GetAutoinfectCount()
{
return m_AutoinfectCount;
}
string GetName()
{
return ClassName();
}
bool GrowDuringAntibioticsAttack(PlayerBase player)
{
return true;
}
}