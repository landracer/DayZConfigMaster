// Source: F:/Games/Dayz/scripts/3_game/progressasync.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ProgressAsync
{
    //proto native void SetUserData(Widget inst);
    proto static native void SetUserData(Widget inst);
    proto static native void SetProgressData(Widget inst);
    proto static native void DestroyAllPendingProgresses();
    proto static native void StartProgress();
