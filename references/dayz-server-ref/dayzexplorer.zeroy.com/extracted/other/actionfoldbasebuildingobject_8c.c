// ======================================================================
// File: actionfoldbasebuildingobject_8c_source.html
// Category: other
// ======================================================================

1class ActionFoldBaseBuildingObjectCB : ActionContinuousBaseCB
2{
3 override void CreateActionComponent()
4 {
5 m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DEPLOY);
6 }
7}
8
9class ActionFoldBaseBuildingObject : ActionContinuousBase
10{
11 void ActionFoldBaseBuildingObject()
12 {
13 m_CallbackClass = ActionFoldBaseBuildingObjectCB;
14 m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
15 m_FullBody = true;
16 m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
17 m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
18
19 m_Text = "#fold";
20 }
21
22 override void CreateConditionComponents()
23 {
24 m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
25 m_ConditionItem = new CCINotPresent();
26 }
27
28 override typename GetInputType()
29 {
30 return ContinuousInteractActionInput;
31 }
32
33 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
34 {
35 Object targetObject = target.GetObject();
36 if (targetObject.CanUseConstruction())
37 {
38 BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(targetObject);
39
40 if (baseBuilding.CanFoldBaseBuildingObject())
41 return true;
42 }
43
44 return false;
45 }
46
47 override void OnStartServer(ActionData action_data)
48 {
49 super.OnStartServer(action_data);
50
51 ItemBase item = ItemBase.Cast(action_data.m_Target.GetObject());
52 if (item.GetLoopFoldSoundset() != string.Empty)
53 item.StartItemSoundServer(SoundConstants.ITEM_FOLD_LOOP);
54 }
55
56 override void OnFinishProgressServer(ActionData action_data)
57 {
58 super.OnFinishProgressServer(action_data);
59
60 BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(action_data.m_Target.GetObject());
61 baseBuilding.CreateConstructionKitInHands(action_data.m_Player);
62 baseBuilding.DestroyConstruction();
63
64 if (baseBuilding.GetFoldSoundset() != string.Empty)
65 baseBuilding.StartItemSoundServer(SoundConstants.ITEM_FOLD);
66 if (baseBuilding)
67 baseBuilding.StopItemSoundServer(SoundConstants.ITEM_FOLD_LOOP);
68 }
69
70 override void OnEndServer(ActionData action_data)
71 {
72 super.OnEndServer(action_data);
73
74 //ItemBase item = ItemBase.Cast(action_data.m_Player.GetItemInHands());
75 ItemBase item = ItemBase.Cast(action_data.m_Target.GetObject());
76 if (item)
77 item.StopItemSoundServer(SoundConstants.ITEM_FOLD_LOOP);
78 }
79
80 override string GetAdminLogMessage(ActionData action_data)
81 {
82 return " folded " + action_data.m_Target.GetObject().GetDisplayName();
83 }
84}