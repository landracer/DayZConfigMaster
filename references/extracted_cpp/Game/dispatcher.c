// Source: F:/Games/Dayz/scripts/3_game/tools/dispatcher.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef int CallID;
 
const CallID CALL_ID_SEND_LOG                           = 0;
const CallID CALL_ID_SCR_CNSL_HISTORY_BACK              = 1;
const CallID CALL_ID_SCR_CNSL_HISTORY_NEXT              = 2;
const CallID CALL_ID_SCR_CNSL_ADD_PRINT                 = 3;
const CallID CALL_ID_SCR_CNSL_GET_SQF_WATCHER           = 4;
const CallID CALL_ID_SCR_CNSL_SET_SQF_WATCHER_RESULT    = 5;
const CallID CALL_ID_SCENE_EDITOR_COMMAND               = 6;
const CallID CALL_ID_HIDE_INVENTORY                     = 7;
const CallID CALL_ID_SCR_CNSL_GETSELECTEDITEM           = 8;
 
class Dispatcher
{   
    Param CallMethod(CallID call_id, Param params){}
