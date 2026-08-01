// ======================================================================
// File: preparefish_8c_source.html
// Category: other
// ======================================================================

class PrepareFish extends PrepareAnimal
{
override void Init()
{
super.Init();
m_Name = "#STR_gutandprepare0";
m_IngredientAddHealth[1] = -4; // 0 = do nothing
}
};