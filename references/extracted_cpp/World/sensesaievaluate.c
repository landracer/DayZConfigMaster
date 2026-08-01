// Source: F:/Games/Dayz/scripts/4_world/static/sensesaievaluate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NoiseAIEvaluate
{
    static float SURFACE_NOISE_WEIGHT = 0.25;
                
    static float GetNoiseMultiplier(DayZPlayerImplement playerImplement)
    {
        float speedNoise    = GetNoiseMultiplierByPlayerSpeed(playerImplement);
        float shoesNoise    = GetNoiseMultiplierByShoes(playerImplement);
        float surfaceNoise  = GetNoiseMultiplierBySurface(playerImplement);
        
        surfaceNoise        *= SURFACE_NOISE_WEIGHT;
        float avgNoise      = (shoesNoise + surfaceNoise)/(1 + SURFACE_NOISE_WEIGHT);
        avgNoise            *= speedNoise;
        
        return avgNoise;
