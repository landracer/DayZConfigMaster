// Source: F:/Games/Dayz/scripts/3_game/client/notifications/notificationsystem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const static float NOTIFICATION_FADE_TIME = 3.0; 
 
enum NotificationType
{
    FRIEND_CONNECTED,
    INVITE_FAIL_SAME_SERVER,
    JOIN_FAIL_GET_SESSION,
    CONNECT_FAIL_GENERIC,
    DISCONNECTED,
    GENERIC_ERROR,
    //Please add types before this item
    NOTIFICATIONS_END
