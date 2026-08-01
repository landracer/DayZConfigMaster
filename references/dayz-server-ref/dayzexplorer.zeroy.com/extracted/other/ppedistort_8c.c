// ======================================================================
// File: ppedistort_8c_source.html
// Category: other
// ======================================================================

class PPEDistort: PPEClassBase
{
static const int PARAM_DISTORT = 0;
static const int PARAM_CENTERX = 1;
static const int PARAM_CENTERY = 2;
static const int PARAM_CHROM_ABB = 3;
static const int PARAM_CHROM_ABB_SHIFTR = 4;
static const int PARAM_CHROM_ABB_SHIFTG = 5;
static const int PARAM_CHROM_ABB_SHIFTB = 6;
static const int PARAM_CHROM_COLOR_MOD = 7;
static const int PARAM_DISTORT_WEIGHT = 8;
//layering info
static const int L_0_HMP = 100;
static const int L_1_HMP = 100;
static const int L_2_HMP = 100;
static const int L_3_HMP = 100;
static const int L_4_HMP = 100;
static const int L_5_HMP = 100;
static const int L_6_HMP = 100;
static const int L_7_HMP = 100;
override int GetPostProcessEffectID()
{
return PostProcessEffectType.Distort;
}
override string GetDefaultMaterialPath()
{
return "Graphics/Materials/postprocess/distort";
}
override void RegisterMaterialParameters()
{
RegisterParameterScalarFloat(PARAM_DISTORT, "Distort", 0, -5, 5);
RegisterParameterScalarFloat(PARAM_CENTERX, "CenterX", 0, -1, 1);
RegisterParameterScalarFloat(PARAM_CENTERY, "CenterY", 0, -1, 1);
RegisterParameterScalarFloat(PARAM_CHROM_ABB, "MaxChromAbberation", 0, 0, 1);
RegisterParameterScalarFloat(PARAM_CHROM_ABB_SHIFTR, "ChromAbberationShiftR", 0, -2, 2);
RegisterParameterScalarFloat(PARAM_CHROM_ABB_SHIFTG, "ChromAbberationShiftG", 1, -2, 2);
RegisterParameterScalarFloat(PARAM_CHROM_ABB_SHIFTB, "ChromAbberationShiftB", 2, -2, 2);
RegisterParameterColor(PARAM_CHROM_COLOR_MOD, "ModulateColor", 0, 0, 0, 0);
RegisterParameterScalarFloat(PARAM_DISTORT_WEIGHT, "DistortWeight", 1.0, 0.0, 1.0);
}
}