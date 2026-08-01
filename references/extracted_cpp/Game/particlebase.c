// Source: F:/Games/Dayz/scripts/3_game/particles/particlebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ParticleEvents
{
    ref ScriptInvoker Event_OnParticleStart = new ScriptInvoker();
    ref ScriptInvoker Event_OnParticleStop = new ScriptInvoker();
    ref ScriptInvoker Event_OnParticleReset = new ScriptInvoker();
    ref ScriptInvoker Event_OnParticleEnd = new ScriptInvoker();
    ref ScriptInvoker Event_OnParticleParented = new ScriptInvoker();
    ref ScriptInvoker Event_OnParticleUnParented = new ScriptInvoker();
