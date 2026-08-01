// Source: F:/Games/Dayz/scripts/5_mission/mission/dispatchercaller.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DispatcherCaller extends Dispatcher
{
    private void SendLogAtClient(Param1<string> p)
    {
        PluginDeveloper m = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
        
        if ( m != NULL )
        {
            m.SendServerLogToClient(p.param1);
        }
