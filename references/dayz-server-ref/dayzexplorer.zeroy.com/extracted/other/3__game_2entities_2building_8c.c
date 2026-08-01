// ======================================================================
// File: 3__game_2entities_2building_8c_source.html
// Category: other
// ======================================================================

1typedef Param1<int> DoorStartParams;
2typedef Param2<int, bool> DoorFinishParams;
3typedef Param1<int> DoorLockParams;
4
5class BuildingType : EntityAIType
6{
7
8};
9
10class Building : EntityAI
11{
12 proto native int GetLaddersCount();
13 proto native vector GetLadderPosTop(int ladderIndex);
14 proto native vector GetLadderPosBottom(int ladderIndex);
15
17 proto native int GetDoorIndex(int componentIndex);
18
20 proto native int GetDoorCount();
21
23 proto native bool IsDoorOpen(int index);
24
26 proto native bool IsDoorOpening(int index);
27
29 proto native bool IsDoorOpeningAjar(int index);
30
32 proto native bool IsDoorClosing(int index);
33
35 proto native bool IsDoorOpened(int index);
36
38 proto native bool IsDoorOpenedAjar(int index);
39
41 proto native bool IsDoorClosed(int index);
42
44 proto native bool IsDoorLocked(int index);
45
47 proto native void PlayDoorSound(int index);
48
50 proto native void OpenDoor(int index);
51
53 proto native void CloseDoor(int index);
54
56 proto native void LockDoor(int index, bool force = false);
57
59 proto native void UnlockDoor(int index, bool animate = true);
60
62 proto native vector GetDoorSoundPos(int index);
63
65 proto native float GetDoorSoundDistance(int index);
66
68 proto native void OutputDoorLog();
69
71 int GetNearestDoorBySoundPos(vector position)
72 {
73 float smallestDist = float.MAX;
74 int nearestDoor = -1;
75
76 int count = GetDoorCount();
77 for (int i = 0; i < count; i++)
78 {
79 float dist = vector.DistanceSq(GetDoorSoundPos(i), position);
80 if (dist < smallestDist)
81 {
82 nearestDoor = i;
83 smallestDist = dist;
84 }
85 }
86
87 return nearestDoor;
88 }
89
91 void OnDoorOpenStart(DoorStartParams params)
92 {
93 }
94
96 void OnDoorOpenFinish(DoorFinishParams params)
97 {
98 }
99
101 void OnDoorOpenAjarStart(DoorStartParams params)
102 {
103 }
104
106 void OnDoorOpenAjarFinish(DoorFinishParams params)
107 {
108 }
109
111 void OnDoorCloseStart(DoorStartParams params)
112 {
113 }
114
116 void OnDoorCloseFinish(DoorFinishParams params)
117 {
118 }
119
121 void OnDoorLocked(DoorLockParams params)
122 {
123 }
124
126 void OnDoorUnlocked(DoorLockParams params)
127 {
128 }
129
130 bool CanDoorBeOpened(int doorIndex, bool checkIfLocked = false)
131 {
132 if (IsDoorOpen(doorIndex))
133 return false;
134
135 if (checkIfLocked)
136 {
137 if (IsDoorLocked(doorIndex))
138 return false;
139 }
140 else
141 {
142 if (!IsDoorLocked(doorIndex))
143 return false;
144 }
145 return true;
146 }
147
148 bool CanDoorBeClosed(int doorIndex)
149 {
150 return IsDoorOpen(doorIndex);
151 }
152
154 bool CanDoorBeLocked(int doorIndex)
155 {
156 return (!IsDoorOpen(doorIndex) && !IsDoorLocked(doorIndex));
157 }
158
167 int GetLockCompatibilityType(int doorIdx)
168 {
169 return 1 << EBuildingLockType.LOCKPICK; //all doors are lockpickable by default
170 }
171
172 override void GetDebugActions(out TSelectableActionInfoArrayEx outputList)
173 {
174 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_OUTPUT_LOG, "Output Door Log", FadeColors.LIGHT_GREY));
175 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
176
177 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_PLAY_DOOR_SOUND, "Play Door Sound", FadeColors.LIGHT_GREY));
178 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_OPEN_DOOR, "Open Door", FadeColors.LIGHT_GREY));
179 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_CLOSE_DOOR, "Close Door", FadeColors.LIGHT_GREY));
180 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_LOCK_DOOR, "Lock Door", FadeColors.LIGHT_GREY));
181 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.BUILDING_UNLOCK_DOOR, "Unlock Door", FadeColors.LIGHT_GREY));
182 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
183
184 if (Gizmo_IsSupported())
185 {
186 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.GIZMO_OBJECT, "Gizmo Object", FadeColors.LIGHT_GREY));
187 outputList.Insert(new TSelectableActionInfoWithColor(SAT_DEBUG_ACTION, EActions.SEPARATOR, "___________________________", FadeColors.RED));
188 }
189
190 super.GetDebugActions(outputList);
191 }
192
193 override bool OnAction(int action_id, Man player, ParamsReadContext ctx)
194 {
195 if (super.OnAction(action_id, player, ctx))
196 return true;
197
198 if (g_Game.IsClient() || !g_Game.IsMultiplayer())
199 {
200 switch (action_id)
201 {
202 case EActions.GIZMO_OBJECT:
203 if (GetGizmoApi())
204 GetGizmoApi().SelectObject(this);
205 return true;
206 }
207 }
208
209 switch (action_id)
210 {
211 case EActions.BUILDING_PLAY_DOOR_SOUND:
212 PlayDoorSound(GetNearestDoorBySoundPos(player.GetPosition()));
213 return true;
214 }
215
216 if (!g_Game.IsServer())
217 return false;
218
219 switch (action_id)
220 {
221 case EActions.BUILDING_OUTPUT_LOG:
222 OutputDoorLog();
223 return true;
224 case EActions.BUILDING_OPEN_DOOR:
225 OpenDoor(GetNearestDoorBySoundPos(player.GetPosition()));
226 return true;
227 case EActions.BUILDING_CLOSE_DOOR:
228 CloseDoor(GetNearestDoorBySoundPos(player.GetPosition()));
229 return true;
230 case EActions.BUILDING_LOCK_DOOR:
231 LockDoor(GetNearestDoorBySoundPos(player.GetPosition()));
232 return true;
233 case EActions.BUILDING_UNLOCK_DOOR:
234 UnlockDoor(GetNearestDoorBySoundPos(player.GetPosition()));
235 return true;
236 }
237
238 return false;
239 }
240
241 override bool IsBuilding()
242 {
243 return true;
244 }
245
246 override bool CanObstruct()
247 {
248 return true;
249 }
250
251 override bool IsHealthVisible()
252 {
253 return false;
254 }
255
256 ref TIntArray m_InteractActions;
257
258 void Building()
259 {
260 m_InteractActions = new TIntArray;
261 g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " InteractActions", m_InteractActions);
262 }
263
264 override bool IsInventoryVisible()
265 {
266 return false;
267 }
268
269 override int GetMeleeTargetType()
270 {
271 return EMeleeTargetType.NONALIGNABLE;
272 }
273};
274
275//-----------------------------------------------------------------------------
276class WindSockType : EntityType
277{
278};
279
280//-----------------------------------------------------------------------------
281/*
282class WindSock : Entity
283{
284};
285*/