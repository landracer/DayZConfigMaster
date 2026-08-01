// ======================================================================
// File: 4__world_2entities_2game_2super_2building_8c_source.html
// Category: world_classes
// ======================================================================

1class BuildingBase : Building
2{
3 static ref map<typename, ref TInputActionMap> m_BuildingTypeActionsMap = new map<typename, ref TInputActionMap>;
4 TInputActionMap m_InputActionMap;
5 bool m_ActionsInitialize;
6
7 void BuildingBase()
8 {
9 m_ActionsInitialize = false;
10 }
11
12 void InitializeActions()
13 {
14 m_InputActionMap = m_BuildingTypeActionsMap.Get( this.Type() );
15 if (!m_InputActionMap)
16 {
17 TInputActionMap iam = new TInputActionMap;
18 m_InputActionMap = iam;
19 SetActions();
20 m_BuildingTypeActionsMap.Insert(this.Type(), m_InputActionMap);
21 }
22 }
23
24 override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
25 {
26 if (!m_ActionsInitialize)
27 {
28 m_ActionsInitialize = true;
29 InitializeActions();
30 }
31
32 actions = m_InputActionMap.Get(action_input_type);
33 }
34
35 void SetActions()
36 {
37 //AddAction();
38 }
39
40 void AddAction(typename actionName)
41 {
42 ActionBase action = ActionManagerBase.GetAction(actionName);
43
44 if (!action)
45 {
46 Debug.LogError("Action " + actionName + " dosn&#39;t exist!");
47 return;
48 }
49
50 typename ai = action.GetInputType();
51 if (!ai)
52 {
53 m_ActionsInitialize = false;
54 return;
55 }
56 ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
57
58 if (!action_array)
59 {
60 action_array = new array<ActionBase_Basic>;
61 m_InputActionMap.Insert(ai, action_array);
62 }
63
64 if ( LogManager.IsActionLogEnable() )
65 {
66 Debug.ActionLog(action.ToString() + " -> " + ai, this.ToString() , "n/a", "Add action" );
67 }
68 action_array.Insert(action);
69 }
70
71 void RemoveAction(typename actionName)
72 {
73 PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
74 ActionBase action = player.GetActionManager().GetAction(actionName);
75 typename ai = action.GetInputType();
76 ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
77
78 if (action_array)
79 {
80 action_array.RemoveItem(action);
81 }
82 }
83};
84
85class House : BuildingBase
86{
87 void House()
88 {
89 }
90};
91
92typedef House BuildingSuper;