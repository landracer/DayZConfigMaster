// ======================================================================
// File: scriptanalytics_8c_source.html
// Category: other
// ======================================================================

// class binded to engine
class StatsEventMeasuresData
{
string m_CharacterId;
int m_TimeInterval;
int m_DaytimeHour;
vector m_PositionStart;
vector m_PositionEnd;
float m_DistanceOnFoot;
float m_DistanceVehicle;
float m_TimeVONIn;
float m_TimeVONOut;
int m_CntLootAny;
int m_CntLootFood;
int m_CntLootCloth;
int m_CntLootFirearm;
int m_CntLootAmmo;
int m_CntKillInfected;
int m_CntConsumedFood;
int m_CntConsumedWater;
int m_HealthRestored;
int m_CntFiredAmmo;
int m_CntCraftedItem;
// listStatus // state metric of health, hunger, thirst, etc... when event is created/send
int m_HealthStatus;
int m_BloodStatus;
int m_SicknessStatus;
int m_TemperatureStatus;
int m_FoodStatus;
int m_HydrationStatus;
};
// class binded to engine
class StatsEventDeathData
{
string m_CharacterId;
int m_CharacterLifetime;
string m_Cause;
string m_WeaponName;
float m_Distance;
vector m_Position;
int m_ListDamages[5];
};
// class binded to engine
class StatsEventScoredKillData
{
string m_CharacterId;
string m_WeaponName;
int m_KillDistance;
vector m_PositionKiller;
vector m_PositionVictim;
};
// class binded to engine
class StatsEventDisconnectedData
{
string m_CharacterId;
string m_Reason;
};
// class binded to engine
class StatsEventSpawnedData
{
string m_CharacterId;
int m_Lifetime;
vector m_Position;
int m_Population;
int m_DaytimeHour;
};
// class binded to engine
class StatsEventData
{
void StatsEventData(string eventName)
{
m_eventName = eventName;
m_valuesBool = new map<string, int>();
m_valuesInt = new map<string, int>();
m_valuesFloat = new map<string, float>();
m_valuesString = new map<string, string>();
m_valuesVector = new map<string, vector>();
}
void AddBool(string key, bool value)
{
m_valuesBool.Insert(key, (int)value);
}
void AddInt(string key, int value)
{
m_valuesInt.Insert(key, value);
}
void AddFloat(string key, float value)
{
m_valuesFloat.Insert(key, value);
}
void AddString(string key, string value)
{
m_valuesString.Insert(key, value);
}
void AddVector(string key, vector value)
{
m_valuesVector.Insert(key, value);
}
private string m_eventName;
private autoptr map<string, int> m_valuesBool;//TODO: use bool instead of int (problem with engine type binding)
private autoptr map<string, int> m_valuesInt;
private autoptr map<string, float> m_valuesFloat;
private autoptr map<string, string> m_valuesString;
private autoptr map<string, vector> m_valuesVector;
}
class ScriptAnalytics
{
static proto native void SendPlayerDeath(StatsEventDeathData data);
static proto native void SendPlayerScoredKill(StatsEventScoredKillData data);
static proto native void SendPlayerDisconnected(StatsEventDisconnectedData data);
static proto native void SendPlayerMeasures(StatsEventMeasuresData data);
static proto native void SendPlayerSpawned(StatsEventSpawnedData data);
static proto native void SendEvent(StatsEventData data);
};
class Analytics
{
// send stats data with log
static void PlayerDeath(StatsEventDeathData data)
{
ScriptAnalytics.SendPlayerDeath(data);
}
// send stats data with log
static void PlayerScoredKill(StatsEventScoredKillData data)
{
ScriptAnalytics.SendPlayerScoredKill(data);
}
// send stats data with log
static void PlayerDisconnected(StatsEventDisconnectedData data)
{
ScriptAnalytics.SendPlayerDisconnected(data);
}
// send stats data with log
static void PlayerMeasures(StatsEventMeasuresData data)
{
ScriptAnalytics.SendPlayerMeasures(data);
}
// send stats data with log
static void PlayerSpawned(StatsEventSpawnedData data)
{
ScriptAnalytics.SendPlayerSpawned(data);
}
}