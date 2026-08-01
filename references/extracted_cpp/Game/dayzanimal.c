// Source: F:/Games/Dayz/scripts/3_game/entities/dayzanimal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZCreatureAnimInterface
{
    private void DayZCreatureAnimInterface() {}
    private void ~DayZCreatureAnimInterface() {}
    
    //-----------------------------------------------------
    // Binds, returns -1 when error, otherwise if ok

    proto native TAnimGraphCommand      BindCommand(string pCommandName);

    proto native TAnimGraphVariable     BindVariableFloat(string pVariable);
    proto native TAnimGraphVariable     BindVariableInt(string pVariable);
    proto native TAnimGraphVariable     BindVariableBool(string pVariable);

    proto native TAnimGraphTag          BindTag(string pTagName);

    proto native TAnimGraphEvent        BindEvent(string pEventName);
