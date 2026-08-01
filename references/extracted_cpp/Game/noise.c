// Source: F:/Games/Dayz/scripts/3_game/noise.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NoiseSystem
{
    private void NoiseSystem() {}
    private void ~NoiseSystem() {}
    
    proto void AddNoise(EntityAI source_entity, NoiseParams noise_params, float external_strenght_multiplier = 1.0);
    proto void AddNoisePos(EntityAI source_entity, vector pos, NoiseParams noise_params, float external_strenght_multiplier = 1.0);
    
    proto void AddNoiseTarget(vector pos, float lifetime, NoiseParams noise_params, float external_strength_multiplier = 1.0);
}
    
class NoiseParams
{   
    void NoiseParams()
    {
        
