// ======================================================================
// File: dayzaihitcomponents_8c_source.html
// Category: other
// ======================================================================

class DayZAIHitComponent
{
string m_Name;
int m_Weight;
};
class DayZAIHitComponentHelpers
{
static void RegisterHitComponent(array<ref DayZAIHitComponent> pHitComponents, string pName, float pWeight)
{
DayZAIHitComponent newComponent = new DayZAIHitComponent();
newComponent.m_Name = pName;
newComponent.m_Weight = pWeight;
pHitComponents.Insert(newComponent);
}
static bool SelectMostProbableHitComponent(array<ref DayZAIHitComponent> pHitComponents, out string pHitComponent)
{
int weights = SumOfWeights(pHitComponents);
float rnd = Math.RandomInt(0, weights);
for ( int i = 0; i < pHitComponents.Count(); ++i )
{
DayZAIHitComponent hitComp = pHitComponents.Get(i);
rnd -= hitComp.m_Weight;
if (rnd <= 0)
{
pHitComponent = hitComp.m_Name;
return true;
}
}
return false;
}
static int SumOfWeights(array<ref DayZAIHitComponent> pHitComponents)
{
int sum = 0;
for( int i = 0; i < pHitComponents.Count(); ++i )
{
DayZAIHitComponent hitComp = pHitComponents.Get(i);
sum += hitComp.m_Weight;
}
return sum;
}
};