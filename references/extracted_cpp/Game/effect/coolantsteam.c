// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/vehiclesmoke/coolantsteam.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EffCoolantSteam : EffVehicleSmoke
{
    override void SetParticleStateLight()
    {
        SetParticleState( ParticleList.HATCHBACK_COOLANT_OVERHEATING );
        UpdateParticle();
