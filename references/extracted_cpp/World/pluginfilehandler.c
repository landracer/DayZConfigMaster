// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginFileHandler extends PluginBase
{
    static bool FileDuplicate(string source_name, string dest_name)
    {
        return CopyFile(source_name, dest_name);
