// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/torch_video.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Torch_Video : LongTorch
{
    protected float PARTICLE_DELAY = 15000;
    protected bool m_EnablePtc;
    
    void SetEnablePtc()
    {
        m_EnablePtc = true;
