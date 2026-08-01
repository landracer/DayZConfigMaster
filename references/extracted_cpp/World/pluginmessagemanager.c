// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginmessagemanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PluginMessageManager extends PluginBase
{
    int channelsUsed = 0;
    ref array<ref MessageReceiverBase>  channelList[NUM_OF_CHANNELS];
    
    void PluginMessageManager()
    {
        for (int i = 0; i < NUM_OF_CHANNELS; i++)
        {
            channelList[i] = new array<ref MessageReceiverBase>;
        }
