// ======================================================================
// File: basebuildingbase_8c_source.html
// Category: other
// ======================================================================

1//BASE BUILDING BASE
2class BaseBuildingBase extends ItemBase
3{
4 const string ANIMATION_DEPLOYED = "Deployed";
5
6 float m_ConstructionKitHealth; //stored health value for used construction kit
7
8 ref Construction m_Construction;
9
10 bool m_HasBase;
11 //variables for synchronization of base building parts (2x31 is the current limit)
12 int m_SyncParts01; //synchronization for already built parts (31 parts)
13 int m_SyncParts02; //synchronization for already built parts (+31 parts)
14 int m_SyncParts03; //synchronization for already built parts (+31 parts)
15 int m_InteractedPartId; //construction part id that an action was performed on
16 int m_PerformedActionId; //action id that was performed on a construction part
17
18 //Sounds
19 //build
20 const string SOUND_BUILD_WOOD_LOG = "putDown_WoodLog_SoundSet";
21 const string SOUND_BUILD_WOOD_PLANK = "putDown_WoodPlank_SoundSet";
22 const string SOUND_BUILD_WOOD_STAIRS = "putDown_WoodStairs_SoundSet";
23 const string SOUND_BUILD_METAL = "putDown_MetalPlank_SoundSet";
24 const string SOUND_BUILD_WIRE = "putDown_BarbedWire_SoundSet";
25 //dismantle
26 const string SOUND_DISMANTLE_WOOD_LOG = "Crash_WoodPlank_SoundSet";
27 const string SOUND_DISMANTLE_WOOD_PLANK = "Crash_WoodPlank_SoundSet";
28 const string SOUND_DISMANTLE_WOOD_STAIRS = "Crash_WoodPlank_SoundSet";
29 const string SOUND_DISMANTLE_METAL = "Crash_MetalPlank_SoundSet";
30 const string SOUND_DISMANTLE_WIRE = "putDown_BarbedWire_SoundSet";
31
32 protected EffectSound m_Sound;
33
34 ref map<string, ref AreaDamageManager> m_DamageTriggers;
35 ref array<string> m_HybridAttachments;
36 ref array<string> m_Mountables;
37
38 // Constructor
39 void BaseBuildingBase()
40 {
41 m_DamageTriggers = new map<string, ref AreaDamageManager>;
42
43 //synchronized variables
44 RegisterNetSyncVariableInt( "m_SyncParts01" );
45 RegisterNetSyncVariableInt( "m_SyncParts02" );
46 RegisterNetSyncVariableInt( "m_SyncParts03" );
47 RegisterNetSyncVariableInt( "m_InteractedPartId" );
48 RegisterNetSyncVariableInt( "m_PerformedActionId" );
49 RegisterNetSyncVariableBool( "m_HasBase" );
50
51 //Construction init
52 ConstructionInit();
53
54 if (ConfigIsExisting("hybridAttachments"))
55 {
56 m_HybridAttachments = new array<string>;
57 ConfigGetTextArray("hybridAttachments", m_HybridAttachments);
58 }
59 if (ConfigIsExisting("mountables"))
60 {
61 m_Mountables = new array<string>;
62 ConfigGetTextArray("mountables", m_Mountables);
63 }
64
65 ProcessInvulnerabilityCheck(GetInvulnerabilityTypeString());
66 }
67
68 override void EEDelete(EntityAI parent)
69 {
70 super.EEDelete(parent);
71
72 foreach (AreaDamageManager areaDamage : m_DamageTriggers)
73 {
74 areaDamage.Destroy();
75 }
76
77 }
78
79 override string GetInvulnerabilityTypeString()
80 {
81 return "disableBaseDamage";
82 }
83
84 override bool CanObstruct()
85 {
86 return true;
87 }
88
89 override int GetHideIconMask()
90 {
91 return EInventoryIconVisibility.HIDE_VICINITY;
92 }
93
94 override void InitItemSounds()
95 {
96 super.InitItemSounds();
97
98 ItemSoundHandler handler = GetItemSoundHandler();
99 SoundParameters params = new SoundParameters();
100 params.m_Loop = true;
101
102 if (GetFoldSoundset() != string.Empty)
103 handler.AddSound(SoundConstants.ITEM_FOLD, GetFoldSoundset());
104 if (GetLoopFoldSoundset() != string.Empty)
105 handler.AddSound(SoundConstants.ITEM_FOLD_LOOP, GetLoopFoldSoundset(), params);
106 }
107
108 override string GetFoldSoundset()
109 {
110 return "putDown_FenceKit_SoundSet";
111 }
112
113 override string GetLoopFoldSoundset()
114 {
115 return "Shelter_Site_Build_Loop_SoundSet";
116 }
117
118 // --- SYNCHRONIZATION
119 void SynchronizeBaseState()
120 {
121 if ( g_Game.IsServer() )
122 {
123 SetSynchDirty();
124 }
125 }
126
127 override void OnVariablesSynchronized()
128 {
129 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " OnVariablesSynchronized");
130 super.OnVariablesSynchronized();
131
132 g_Game.GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( OnSynchronizedClient, 100, false );
133 }
134
135 protected void OnSynchronizedClient()
136 {
137 //update parts
138 SetPartsFromSyncData();
139
140 //update action on part
141 SetActionFromSyncData();
142
143 //update visuals (client)
144 UpdateVisuals();
145 }
146
147 //parts synchronization
148 void RegisterPartForSync( int part_id )
149 {
150 //part_id must starts from index = 1
151 int offset;
152 int mask;
153
154 if ( part_id >= 1 && part_id <= 31 ) //<1,31> (31 parts)
155 {
156 offset = part_id - 1;
157 mask = 1 << offset;
158
159 m_SyncParts01 = m_SyncParts01 | mask;
160 }
161 else if ( part_id >= 32 && part_id <= 62 ) //<32,62> (31 parts)
162 {
163 offset = ( part_id % 32 );
164 mask = 1 << offset;
165
166 m_SyncParts02 = m_SyncParts02 | mask;
167 }
168 else if ( part_id >= 63 && part_id <= 93 ) //<63,93> (31 parts)
169 {
170 offset = ( part_id % 63 );
171 mask = 1 << offset;
172
173 m_SyncParts03 = m_SyncParts03 | mask;
174 }
175 }
176
177 void UnregisterPartForSync( int part_id )
178 {
179 //part_id must starts from index = 1
180 int offset;
181 int mask;
182
183 if ( part_id >= 1 && part_id <= 31 ) //<1,31> (31 parts)
184 {
185 offset = part_id - 1;
186 mask = 1 << offset;
187
188 m_SyncParts01 = m_SyncParts01 & ~mask;
189 }
190 else if ( part_id >= 32 && part_id <= 62 ) //<32,62> (31 parts)
191 {
192 offset = ( part_id % 32 );
193 mask = 1 << offset;
194
195 m_SyncParts02 = m_SyncParts02 & ~mask;
196 }
197 else if ( part_id >= 63 && part_id <= 93 ) //<63,93> (31 parts)
198 {
199 offset = ( part_id % 63 );
200 mask = 1 << offset;
201
202 m_SyncParts03 = m_SyncParts03 & ~mask;
203 }
204 }
205
206 bool IsPartBuildInSyncData( int part_id )
207 {
208 //part_id must starts from index = 1
209 int offset;
210 int mask;
211
212 if ( part_id >= 1 && part_id <= 31 ) //<1,31> (31 parts)
213 {
214 offset = part_id - 1;
215 mask = 1 << offset;
216
217 if ( ( m_SyncParts01 & mask ) > 0 )
218 {
219 return true;
220 }
221 }
222 else if ( part_id >= 32 && part_id <= 62 ) //<32,62> (31 parts)
223 {
224 offset = ( part_id % 32 );
225 mask = 1 << offset;
226
227 if ( ( m_SyncParts02 & mask ) > 0 )
228 {
229 return true;
230 }
231 }
232 else if ( part_id >= 63 && part_id <= 93 ) //<63,93> (31 parts)
233 {
234 offset = ( part_id % 63 );
235 mask = 1 << offset;
236
237 if ( ( m_SyncParts03 & mask ) > 0 )
238 {
239 return true;
240 }
241 }
242
243 return false;
244 }
245
246 protected void RegisterActionForSync( int part_id, int action_id )
247 {
248 m_InteractedPartId = part_id;
249 m_PerformedActionId = action_id;
250 }
251
252 protected void ResetActionSyncData()
253 {
254 //reset data
255 m_InteractedPartId = -1;
256 m_PerformedActionId = -1;
257 }
258
259 protected void SetActionFromSyncData()
260 {
261 if ( m_InteractedPartId > -1 && m_PerformedActionId > -1 )
262 {
263 ConstructionPart constrution_part = GetConstructionPartById( m_InteractedPartId );
264 int build_action_id = m_PerformedActionId;
265
266 switch( build_action_id )
267 {
268 case AT_BUILD_PART : OnPartBuiltClient( constrution_part.GetPartName(), build_action_id ); break;
269 case AT_DISMANTLE_PART : OnPartDismantledClient( constrution_part.GetPartName(), build_action_id ); break;
270 case AT_DESTROY_PART : OnPartDestroyedClient( constrution_part.GetPartName(), build_action_id ); break;
271 }
272 }
273 }
274 //------
275
276 void SetPartFromSyncData( ConstructionPart part )
277 {
278 string key = part.m_PartName;
279 bool is_base = part.IsBase();
280 bool is_part_built_sync = IsPartBuildInSyncData( part.GetId() );
281 bsbDebugSpam("[bsb] " + GetDebugName(this) + " SetPartFromSyncData try to sync: built=" + is_part_built_sync + " key=" + key + " part=" + part.GetPartName() + " part_built=" + part.IsBuilt());
282 if ( is_part_built_sync )
283 {
284 if ( !part.IsBuilt() )
285 {
286 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " SetPartsFromSyncData +++ " + key);
287 GetConstruction().AddToConstructedParts( key );
288 GetConstruction().ShowConstructionPartPhysics(part.GetPartName());
289
290 if (is_base)
291 {
292 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " RM");
293 RemoveProxyPhysics( ANIMATION_DEPLOYED );
294 }
295 }
296 }
297 else
298 {
299 if ( part.IsBuilt() )
300 {
301 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " SetPartsFromSyncData --- " + key);
302 GetConstruction().RemoveFromConstructedParts( key );
303 GetConstruction().HideConstructionPartPhysics(part.GetPartName());
304
305 if (is_base)
306 {
307 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " ADD");
308 AddProxyPhysics( ANIMATION_DEPLOYED );
309 }
310 }
311 }
312
313 //check slot lock for material attachments
314 GetConstruction().SetLockOnAttachedMaterials( part.GetPartName(), part.IsBuilt() ); //failsafe for corrupted sync/storage data
315 }
316
317 //set construction parts based on synchronized data
318 void SetPartsFromSyncData()
319 {
320 Construction construction = GetConstruction();
321 map<string, ref ConstructionPart> construction_parts = construction.GetConstructionParts();
322
323 for ( int i = 0; i < construction_parts.Count(); ++i )
324 {
325 string key = construction_parts.GetKey( i );
326 ConstructionPart value = construction_parts.Get( key );
327 SetPartFromSyncData(value);
328 }
329
330 //regenerate navmesh
331 UpdateNavmesh();
332 }
333
334 protected ConstructionPart GetConstructionPartById( int id )
335 {
336 Construction construction = GetConstruction();
337 map<string, ref ConstructionPart> construction_parts = construction.GetConstructionParts();
338
339 for ( int i = 0; i < construction_parts.Count(); ++i )
340 {
341 string key = construction_parts.GetKey( i );
342 ConstructionPart value = construction_parts.Get( key );
343
344 if ( value.GetId() == id )
345 {
346 return value;
347 }
348 }
349
350 return NULL;
351 }
352 //
353
354 //Base
355 bool HasBase()
356 {
357 return m_HasBase;
358 }
359
360 void SetBaseState( bool has_base )
361 {
362 m_HasBase = has_base;
363 }
364
365 override bool IsDeployable()
366 {
367 return true;
368 }
369
370 bool IsOpened()
371 {
372 return false;
373 }
374
375 //--- CONSTRUCTION KIT
376 ItemBase CreateConstructionKit()
377 {
378 ItemBase construction_kit = ItemBase.Cast( g_Game.CreateObjectEx( GetConstructionKitType(), GetKitSpawnPosition(), ECE_PLACE_ON_SURFACE ) );
379 if ( m_ConstructionKitHealth > 0 )
380 {
381 construction_kit.SetHealth( m_ConstructionKitHealth );
382 }
383
384 return construction_kit;
385 }
386
387 void CreateConstructionKitInHands(notnull PlayerBase player)
388 {
389 ItemBase construction_kit = ItemBase.Cast(player.GetHumanInventory().CreateInHands(GetConstructionKitType()));
390 if ( m_ConstructionKitHealth > 0 )
391 {
392 construction_kit.SetHealth( m_ConstructionKitHealth );
393 }
394 }
395
396 protected vector GetKitSpawnPosition()
397 {
398 return GetPosition();
399 }
400
401 protected string GetConstructionKitType()
402 {
403 return "";
404 }
405
406 void DestroyConstructionKit( ItemBase construction_kit )
407 {
408 m_ConstructionKitHealth = construction_kit.GetHealth();
409 g_Game.ObjectDelete( construction_kit );
410 }
411
412 //--- CONSTRUCTION
413 void DestroyConstruction()
414 {
415 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " DestroyConstruction");
416 g_Game.ObjectDelete( this );
417 }
418
419 // --- EVENTS
420 override void OnStoreSave( ParamsWriteContext ctx )
421 {
422 super.OnStoreSave( ctx );
423
424 //sync parts 01
425 ctx.Write( m_SyncParts01 );
426 ctx.Write( m_SyncParts02 );
427 ctx.Write( m_SyncParts03 );
428
429 ctx.Write( m_HasBase );
430 }
431
432 override bool OnStoreLoad( ParamsReadContext ctx, int version )
433 {
434 if ( !super.OnStoreLoad( ctx, version ) )
435 return false;
436
437 //--- Base building data ---
438 //Restore synced parts data
439 if ( !ctx.Read( m_SyncParts01 ) )
440 {
441 m_SyncParts01 = 0; //set default
442 return false;
443 }
444 if ( !ctx.Read( m_SyncParts02 ) )
445 {
446 m_SyncParts02 = 0; //set default
447 return false;
448 }
449 if ( !ctx.Read( m_SyncParts03 ) )
450 {
451 m_SyncParts03 = 0; //set default
452 return false;
453 }
454
455 //has base
456 if ( !ctx.Read( m_HasBase ) )
457 {
458 m_HasBase = false;
459 return false;
460 }
461 //---
462
463 return true;
464 }
465
466 override void AfterStoreLoad()
467 {
468 super.AfterStoreLoad();
469
470 if (!m_FixDamageSystemInit)
471 {
472 SetPartsAfterStoreLoad();
473 }
474 }
475
476 void SetPartsAfterStoreLoad()
477 {
478 //update server data
479 SetPartsFromSyncData();
480
481 //set base state
482 ConstructionPart construction_part = GetConstruction().GetBaseConstructionPart();
483 SetBaseState( construction_part.IsBuilt() ) ;
484
485 //synchronize after load
486 SynchronizeBaseState();
487 }
488
489 override void OnCreatePhysics()
490 {
491 super.OnCreatePhysics();
492 ConstructionInit();
493 SetPartsAfterStoreLoad();
494 }
495
496 override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
497 {
498 if (m_FixDamageSystemInit)
499 return;
500
501 super.EEHealthLevelChanged(oldLevel,newLevel,zone);
502
503 if (g_Game.IsMultiplayer() && !g_Game.IsServer())
504 return;
505
506 Construction construction = GetConstruction();
507 string part_name = zone;
508 part_name.ToLower();
509
510 if ( newLevel == GameConstants.STATE_RUINED )
511 {
512 ConstructionPart construction_part = construction.GetConstructionPart( part_name );
513
514 if ( construction_part && construction.IsPartConstructed( part_name ) )
515 {
516 construction.DestroyPartServer( null, part_name, AT_DESTROY_PART );
517 construction.DestroyConnectedParts(part_name);
518 }
519
520 //barbed wire handling (hack-ish)
521 if ( part_name.Contains("barbed") )
522 {
523 BarbedWire barbed_wire = BarbedWire.Cast( FindAttachmentBySlotName( zone ) );
524 if (barbed_wire)
525 barbed_wire.SetMountedState( false );
526 }
527 }
528 }
529
530 override void EEOnAfterLoad()
531 {
532 if (m_FixDamageSystemInit)
533 {
534 g_Game.GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( SetPartsAfterStoreLoad, 500, false, this );
535 }
536
537 super.EEOnAfterLoad();
538 }
539
540 override void EEInit()
541 {
542 super.EEInit();
543
544 // init visuals and physics
545 InitBaseState();
546
547 //debug
548 #ifdef DEVELOPER
549 DebugCustomState();
550 #endif
551 }
552
553 override void EEItemAttached( EntityAI item, string slot_name )
554 {
555 super.EEItemAttached( item, slot_name );
556
557 CheckForHybridAttachments( item, slot_name );
558 UpdateVisuals();
559 UpdateAttachmentPhysics( slot_name, false );
560 }
561
562 override void EEItemDetached( EntityAI item, string slot_name )
563 {
564 super.EEItemDetached( item, slot_name );
565
566 UpdateVisuals();
567 UpdateAttachmentPhysics( slot_name, false );
568 }
569
570 protected void OnSetSlotLock( int slotId, bool locked, bool was_locked )
571 {
572 string slot_name = InventorySlots.GetSlotName( slotId );
573 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint( "inv: OnSetSlotLock " + GetDebugName( this ) + " slot=" + slot_name + " locked=" + locked + " was_locked=" + was_locked );
574
575 UpdateAttachmentVisuals( slot_name, locked );
576 UpdateAttachmentPhysics( slot_name, locked );
577 }
578
579 //ignore out of reach condition
580 override bool IgnoreOutOfReachCondition()
581 {
582 return true;
583 }
584
585 //CONSTRUCTION EVENTS
586 //Build
587 void OnPartBuiltServer(notnull Man player, string part_name, int action_id)
588 {
589 ConstructionPart construtionPart = GetConstruction().GetConstructionPart(part_name);
590
591 //check base state
592 if (construtionPart.IsBase())
593 {
594 SetBaseState(true);
595
596 //spawn kit
597 CreateConstructionKit();
598 }
599
600 //register constructed parts for synchronization
601 RegisterPartForSync(construtionPart.GetId());
602
603 //register action that was performed on part
604 RegisterActionForSync(construtionPart.GetId(), action_id);
605
606 //synchronize
607 SynchronizeBaseState();
608
609 SetPartFromSyncData(construtionPart); // server part of sync, client will be synced from SetPartsFromSyncData
610
611 UpdateNavmesh();
612
613 //update visuals
614 UpdateVisuals();
615
616 //reset action sync data
617 g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false, this);
618 }
619
620 void OnPartBuiltClient(string part_name, int action_id)
621 {
622 //play sound
623 SoundBuildStart( part_name );
624 }
625
626 //Dismantle
627 void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
628 {
629 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " OnPartDismantledServer " + part_name);
630 ConstructionPart construtionPart = GetConstruction().GetConstructionPart(part_name);
631
632 //register constructed parts for synchronization
633 UnregisterPartForSync(construtionPart.GetId());
634
635 //register action that was performed on part
636 RegisterActionForSync(construtionPart.GetId(), action_id);
637
638 //synchronize
639 SynchronizeBaseState();
640
641 // server part of sync, client will be synced from SetPartsFromSyncData
642 SetPartFromSyncData(construtionPart);
643
644 UpdateNavmesh();
645
646 //update visuals
647 UpdateVisuals();
648
649 //reset action sync data
650 g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false, this);
651
652 //check base state
653 if (construtionPart.IsBase())
654 {
655 //Destroy construction
656 g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DestroyConstruction, 200, false, this);
657 }
658 }
659
660 void OnPartDismantledClient( string part_name, int action_id )
661 {
662 //play sound
663 SoundDismantleStart( part_name );
664 }
665
666 //Destroy
667 void OnPartDestroyedServer(Man player, string part_name, int action_id, bool destroyed_by_connected_part = false)
668 {
669 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " OnPartDestroyedServer " + part_name);
670 ConstructionPart construtionPart = GetConstruction().GetConstructionPart(part_name);
671
672 //register constructed parts for synchronization
673 UnregisterPartForSync(construtionPart.GetId());
674
675 //register action that was performed on part
676 RegisterActionForSync(construtionPart.GetId(), action_id);
677
678 //synchronize
679 SynchronizeBaseState();
680
681 // server part of sync, client will be synced from SetPartsFromSyncData
682 SetPartFromSyncData(construtionPart);
683
684 UpdateNavmesh();
685
686 //update visuals
687 UpdateVisuals();
688
689 //reset action sync data
690 g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false, this);
691
692 //check base state
693 if (construtionPart.IsBase())
694 {
695 //Destroy construction
696 g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DestroyConstruction, 200, false, this);
697 }
698 }
699
700 void OnPartDestroyedClient( string part_name, int action_id )
701 {
702 //play sound
703 SoundDestroyStart( part_name );
704 }
705
707 protected void HandleItemFalling(ConstructionPart part)
708 {
709 bool process = false;
710
711 //TODO: add a parameter to parts&#39; config classes?
712 process |= part.m_PartName.Contains("_roof");
713 process |= part.m_PartName.Contains("_platform");
714 process |= part.m_PartName.Contains("_stair");
715
716 if (process)
717 {
718 if (!MemoryPointExists(part.m_PartName + "_min") || !MemoryPointExists(part.m_PartName + "_max"))
719 {
720 Debug.Log("min/max memory points do not exist for part: " + part.m_PartName);
721 return;
722 }
723
724 vector mins, maxs;
725 mins = ModelToWorld(GetMemoryPointPos(part.m_PartName + "_min"));
726 maxs = ModelToWorld(GetMemoryPointPos(part.m_PartName + "_max"));
727
728 //sanitize minmaxs
729 vector minTmp, maxTmp;
730 minTmp[0] = Math.Min(mins[0],maxs[0]);
731 maxTmp[0] = Math.Max(mins[0],maxs[0]);
732 minTmp[1] = Math.Min(mins[1],maxs[1]);
733 maxTmp[1] = Math.Max(mins[1],maxs[1]);
734 minTmp[2] = Math.Min(mins[2],maxs[2]);
735 maxTmp[2] = Math.Max(mins[2],maxs[2]);
736 mins = minTmp;
737 maxs = maxTmp;
738
739 maxs[1] = maxs[1] + 0.35; //reach a little above..
740
741 ItemFall(mins,maxs);
742 }
743 }
744
746 protected void ItemFall(vector min, vector max)
747 {
748 array<EntityAI> foundEntities = new array<EntityAI>();
749 DayZPlayerUtils.PhysicsGetEntitiesInBox(min,max,foundEntities);
750
751 //filtering
752 ItemBase item;
753 foreach (EntityAI entity : foundEntities)
754 {
755 if (Class.CastTo(item,entity) && !item.CanUseConstruction()) //BB items?
756 item.ThrowPhysically(null,vector.Zero);
757 }
758 }
759
760 // --- UPDATE
761 void InitBaseState()
762 {
763 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " BaseBuildingBase::InitBaseState ");
764
765 InitVisuals();
766 UpdateNavmesh(); //regenerate navmesh
767 GetConstruction().InitBaseState();
768 }
769
770 void InitVisuals()
771 {
772 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " InitVisuals");
773 //check base
774 if ( !HasBase() )
775 {
776 SetAnimationPhase( ANIMATION_DEPLOYED, 0 );
777 }
778 else
779 {
780 SetAnimationPhase( ANIMATION_DEPLOYED, 1 );
781 }
782
783 GetConstruction().UpdateVisuals();
784 }
785
786 void UpdateVisuals()
787 {
788 array<string> attachmentSlots = new array<string>;
789
790 GetAttachmentSlots(this, attachmentSlots);
791 foreach (string slotName : attachmentSlots)
792 {
793 UpdateAttachmentVisuals(slotName, IsAttachmentSlotLocked(slotName));
794 }
795
796 //check base
797 if (!HasBase())
798 SetAnimationPhase(ANIMATION_DEPLOYED, 0);
799 else
800 SetAnimationPhase(ANIMATION_DEPLOYED, 1);
801
802 GetConstruction().UpdateVisuals();
803 }
804
805 void UpdateAttachmentVisuals(string slot_name, bool is_locked)
806 {
807 string slotNameMounted = slot_name + "_Mounted";
808 EntityAI attachment = FindAttachmentBySlotName(slot_name);
809
810 if (attachment)
811 {
812 BarbedWire barbedWire = BarbedWire.Cast(attachment);
813 if (barbedWire && barbedWire.IsMounted())
814 CreateAreaDamage(slotNameMounted);
815 else
816 DestroyAreaDamage(slotNameMounted);
817
818 if (is_locked)
819 {
820 SetAnimationPhase(slotNameMounted, 0);
821 SetAnimationPhase(slot_name, 1);
822 }
823 else
824 {
825 SetAnimationPhase(slotNameMounted, 1);
826 SetAnimationPhase(slot_name, 0);
827 }
828 }
829 else
830 {
831 SetAnimationPhase(slotNameMounted, 1);
832 SetAnimationPhase(slot_name, 1);
833
834 DestroyAreaDamage(slotNameMounted);
835 }
836 }
837
838 // avoid calling this function on frequent occasions, it&#39;s a massive performance hit
839 void UpdatePhysics()
840 {
841 if (LogManager.IsBaseBuildingLogEnable())
842 bsbDebugPrint("[bsb] " + GetDebugName(this) + " BaseBuildingBase::UpdatePhysics");
843
844 array<string> attachmentSlots = new array<string>;
845 GetAttachmentSlots(this, attachmentSlots);
846
847 if (LogManager.IsBaseBuildingLogEnable())
848 bsbDebugPrint("[bsb] " + GetDebugName(this) + " att_cnt=" + attachmentSlots.Count());
849
850 foreach (string slotName : attachmentSlots)
851 {
852 UpdateAttachmentPhysics(slotName, IsAttachmentSlotLocked(slotName));
853 }
854
855 //check base
856 if (!HasBase())
857 {
858 if (LogManager.IsBaseBuildingLogEnable())
859 bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " ADD");
860
861 AddProxyPhysics(ANIMATION_DEPLOYED);
862 }
863 else
864 {
865 if (LogManager.IsBaseBuildingLogEnable())
866 bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " RM");
867
868 RemoveProxyPhysics(ANIMATION_DEPLOYED);
869 }
870
871 GetConstruction().UpdatePhysics();
872 UpdateNavmesh();
873 }
874
875 void UpdateAttachmentPhysics( string slot_name, bool is_locked )
876 {
877 //checks for invalid appends; hotfix
878 if ( !m_Mountables || m_Mountables.Find(slot_name) == -1 )
879 return;
880 //----------------------------------
881 string slot_name_mounted = slot_name + "_Mounted";
882 EntityAI attachment = FindAttachmentBySlotName( slot_name );
883
884 //remove proxy physics
885 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " Removing ATT SLOT=" + slot_name + " RM / RM");
886 RemoveProxyPhysics( slot_name_mounted );
887 RemoveProxyPhysics( slot_name );
888
889 if ( attachment )
890 {
891 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " Adding ATT=" + Object.GetDebugName(attachment));
892 if ( is_locked )
893 {
894 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " RM / RM");
895 AddProxyPhysics( slot_name_mounted );
896 }
897 else
898 {
899 if (LogManager.IsBaseBuildingLogEnable()) bsbDebugPrint("[bsb] " + GetDebugName(this) + " ADD");
900 AddProxyPhysics( slot_name );
901 }
902 }
903 }
904
905 protected void UpdateNavmesh()
906 {
907 SetAffectPathgraph( true, false );
908 g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( g_Game.UpdatePathgraphRegionByObject, 100, false, this );
909 }
910
911 override bool CanUseConstruction()
912 {
913 return true;
914 }
915
916 override bool CanUseConstructionBuild()
917 {
918 return true;
919 }
920
921 protected bool IsAttachmentSlotLocked( EntityAI attachment )
922 {
923 if ( attachment )
924 {
925 InventoryLocation inventory_location = new InventoryLocation;
926 attachment.GetInventory().GetCurrentInventoryLocation( inventory_location );
927
928 return GetInventory().GetSlotLock( inventory_location.GetSlot() );
929 }
930
931 return false;
932 }
933
934 protected bool IsAttachmentSlotLocked( string slot_name )
935 {
936 return GetInventory().GetSlotLock( InventorySlots.GetSlotIdFromString( slot_name ) );
937 }
938
939 //--- ATTACHMENT SLOTS
940 void GetAttachmentSlots( EntityAI entity, out array<string> attachment_slots )
941 {
942 string config_path = "CfgVehicles" + " " + entity.GetType() + " " + "attachments";
943 if ( g_Game.ConfigIsExisting( config_path ) )
944 {
945 g_Game.ConfigGetTextArray( config_path, attachment_slots );
946 }
947 }
948
949 bool CheckSlotVerticalDistance( int slot_id, PlayerBase player )
950 {
951 return true;
952 }
953
954 protected bool CheckMemoryPointVerticalDistance( float max_dist, string selection, PlayerBase player )
955 {
956 return true;
957 }
958
959 protected bool CheckLevelVerticalDistance( float max_dist, string selection, PlayerBase player )
960 {
961 return true;
962 }
963
964 // --- INIT
965 void ConstructionInit()
966 {
967 if ( !m_Construction )
968 {
969 m_Construction = new Construction( this );
970 }
971
972 GetConstruction().Init();
973 }
974
975 Construction GetConstruction()
976 {
977 return m_Construction;
978 }
979
980 //--- INVENTORY/ATTACHMENTS CONDITIONS
981 //attachments
982 override bool CanReceiveAttachment( EntityAI attachment, int slotId )
983 {
984 return super.CanReceiveAttachment(attachment, slotId);
985 }
986
987 bool HasAttachmentsBesidesBase()
988 {
989 int attachment_count = GetInventory().AttachmentCount();
990 if ( attachment_count > 0 )
991 {
992 if ( HasBase() && attachment_count == 1 )
993 {
994 return false;
995 }
996
997 return true;
998 }
999
1000 return false;
1001 }
1002
1003 override bool ShowZonesHealth()
1004 {
1005 return true;
1006 }
1007
1008 override bool IsTakeable()
1009 {
1010 return false;
1011 }
1012
1013 //this into/outo parent.Cargo
1014 override bool CanPutInCargo( EntityAI parent )
1015 {
1016 return false;
1017 }
1018
1019 override bool CanRemoveFromCargo( EntityAI parent )
1020 {
1021 return false;
1022 }
1023
1024 //hands
1025 override bool CanPutIntoHands( EntityAI parent )
1026 {
1027 return false;
1028 }
1029
1030 //--- ACTION CONDITIONS
1031 //direction
1032 override bool IsFacingPlayer( PlayerBase player, string selection )
1033 {
1034 return true;
1035 }
1036
1037 override bool IsPlayerInside( PlayerBase player, string selection )
1038 {
1039 return true;
1040 }
1041
1043 bool MustBeBuiltFromOutside()
1044 {
1045 return false;
1046 }
1047
1048 //camera direction check
1049 bool IsFacingCamera( string selection )
1050 {
1051 return true;
1052 }
1053
1054 //roof check
1055 bool PerformRoofCheckForBase( string partName, PlayerBase player, out bool result )
1056 {
1057 return false;
1058 }
1059
1060 //selection->player distance check
1061 bool HasProperDistance( string selection, PlayerBase player )
1062 {
1063 return true;
1064 }
1065
1066 //folding
1067 bool CanFoldBaseBuildingObject()
1068 {
1069 if ( HasBase() || GetInventory().AttachmentCount() > 0 )
1070 {
1071 return false;
1072 }
1073
1074 return true;
1075 }
1076
1077 ItemBase FoldBaseBuildingObject()
1078 {
1079 ItemBase item = CreateConstructionKit();
1080 DestroyConstruction();
1081
1082 return item;
1083 }
1084
1085 //Damage triggers (barbed wire)
1086 void CreateAreaDamage( string slot_name, float rotation_angle = 0 )
1087 {
1088 if ( g_Game && g_Game.IsServer() )
1089 {
1090 //destroy area damage if some already exists
1091 DestroyAreaDamage( slot_name );
1092
1093 //create new area damage
1094 AreaDamageLoopedDeferred_NoVehicle areaDamage = new AreaDamageLoopedDeferred_NoVehicle( this );
1095 areaDamage.SetDamageComponentType(AreaDamageComponentTypes.HITZONE);
1096
1097 vector min_max[2];
1098 if ( MemoryPointExists( slot_name + "_min" ) )
1099 {
1100 min_max[0] = GetMemoryPointPos( slot_name + "_min" );
1101 }
1102 if ( MemoryPointExists( slot_name + "_max" ) )
1103 {
1104 min_max[1] = GetMemoryPointPos( slot_name + "_max" );
1105 }
1106
1107 //get proper trigger extents (min<max)
1108 vector extents[2];
1109 GetConstruction().GetTriggerExtents( min_max, extents );
1110
1111 //get box center
1112 vector center;
1113 center = GetConstruction().GetBoxCenter( min_max );
1114 center = ModelToWorld( center );
1115
1116 //rotate center if needed
1117 vector orientation = GetOrientation();;
1118 CalcDamageAreaRotation( rotation_angle, center, orientation );
1119
1120 areaDamage.SetExtents( extents[0], extents[1] );
1121 areaDamage.SetAreaPosition( center );
1122 areaDamage.SetAreaOrientation( orientation );
1123 areaDamage.SetLoopInterval( 1.0 );
1124 areaDamage.SetDeferDuration( 0.2 );
1125 areaDamage.SetHitZones( { "Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
1126 areaDamage.SetAmmoName( "BarbedWireHit" );
1127 areaDamage.Spawn();
1128
1129 m_DamageTriggers.Insert( slot_name, areaDamage );
1130 }
1131 }
1132
1133 void CalcDamageAreaRotation( float angle_deg, out vector center, out vector orientation )
1134 {
1135 if ( angle_deg != 0 )
1136 {
1137 //orientation
1138 orientation[0] = orientation[0] - angle_deg;
1139
1140 //center
1141 vector rotate_axis;
1142 if ( MemoryPointExists( "rotate_axis" ) )
1143 {
1144 rotate_axis = ModelToWorld( GetMemoryPointPos( "rotate_axis" ) );
1145 }
1146 float r_center_x = ( Math.Cos( angle_deg * Math.DEG2RAD ) * ( center[0] - rotate_axis[0] ) ) - ( Math.Sin( angle_deg * Math.DEG2RAD ) * ( center[2] - rotate_axis[2] ) ) + rotate_axis[0];
1147 float r_center_z = ( Math.Sin( angle_deg * Math.DEG2RAD ) * ( center[0] - rotate_axis[0] ) ) + ( Math.Cos( angle_deg * Math.DEG2RAD ) * ( center[2] - rotate_axis[2] ) ) + rotate_axis[2];
1148 center[0] = r_center_x;
1149 center[2] = r_center_z;
1150 }
1151 }
1152
1153 void DestroyAreaDamage( string slot_name )
1154 {
1155 if (g_Game && g_Game.IsServer())
1156 {
1157 AreaDamageLoopedDeferred_NoVehicle areaDamage;
1158 if (m_DamageTriggers.Find(slot_name, areaDamage))
1159 {
1160 if (areaDamage)
1161 {
1162 areaDamage.Destroy();
1163 }
1164
1165 m_DamageTriggers.Remove( slot_name );
1166 }
1167 }
1168 }
1169
1170 override bool IsIgnoredByConstruction()
1171 {
1172 return true;
1173 }
1174
1175 //================================================================
1176 // SOUNDS
1177 //================================================================
1178 protected void SoundBuildStart( string part_name )
1179 {
1180 PlaySoundSet( m_Sound, GetBuildSoundByMaterial( part_name ), 0.1, 0.1 );
1181 }
1182
1183 protected void SoundDismantleStart( string part_name )
1184 {
1185 PlaySoundSet( m_Sound, GetDismantleSoundByMaterial( part_name ), 0.1, 0.1 );
1186 }
1187
1188 protected void SoundDestroyStart( string part_name )
1189 {
1190 PlaySoundSet( m_Sound, GetDismantleSoundByMaterial( part_name ), 0.1, 0.1 );
1191 }
1192
1193 protected string GetBuildSoundByMaterial( string part_name )
1194 {
1195 ConstructionMaterialType material_type = GetConstruction().GetMaterialType( part_name );
1196
1197 switch ( material_type )
1198 {
1199 case ConstructionMaterialType.MATERIAL_LOG: return SOUND_BUILD_WOOD_LOG;
1200 case ConstructionMaterialType.MATERIAL_WOOD: return SOUND_BUILD_WOOD_PLANK;
1201 case ConstructionMaterialType.MATERIAL_STAIRS: return SOUND_BUILD_WOOD_STAIRS;
1202 case ConstructionMaterialType.MATERIAL_METAL: return SOUND_BUILD_METAL;
1203 case ConstructionMaterialType.MATERIAL_WIRE: return SOUND_BUILD_WIRE;
1204 }
1205
1206 return "";
1207 }
1208
1209 protected string GetDismantleSoundByMaterial( string part_name )
1210 {
1211 ConstructionMaterialType material_type = GetConstruction().GetMaterialType( part_name );
1212
1213 switch ( material_type )
1214 {
1215 case ConstructionMaterialType.MATERIAL_LOG: return SOUND_DISMANTLE_WOOD_LOG;
1216 case ConstructionMaterialType.MATERIAL_WOOD: return SOUND_DISMANTLE_WOOD_PLANK;
1217 case ConstructionMaterialType.MATERIAL_STAIRS: return SOUND_DISMANTLE_WOOD_STAIRS;
1218 case ConstructionMaterialType.MATERIAL_METAL: return SOUND_DISMANTLE_METAL;
1219 case ConstructionMaterialType.MATERIAL_WIRE: return SOUND_DISMANTLE_WIRE;
1220 }
1221
1222 return "";
1223 }
1224
1225 //misc
1226 void CheckForHybridAttachments( EntityAI item, string slot_name )
1227 {
1228 if (!g_Game.IsMultiplayer() || g_Game.IsServer())
1229 {
1230 //if this is a hybrid attachment, set damage of appropriate damage zone. Zone name must match slot name...WIP
1231 if (m_HybridAttachments && m_HybridAttachments.Find(slot_name) != -1)
1232 {
1233 SetHealth(slot_name,"Health",item.GetHealth());
1234 }
1235 }
1236 }
1237
1238 override int GetDamageSystemVersionChange()
1239 {
1240 return 111;
1241 }
1242
1243 override void SetActions()
1244 {
1245 super.SetActions();
1246
1247 AddAction(ActionDetachFromTarget);
1248 RemoveAction(ActionTakeItem);
1249 RemoveAction(ActionTakeItemToHands);
1250 }
1251
1252 //================================================================
1253 // DEBUG
1254 //================================================================
1255 protected void DebugCustomState()
1256 {
1257 }
1258
1260 array<string> OnDebugSpawnBuildExcludes()
1261 {
1262 return null;
1263 }
1264
1265 override void OnDebugSpawn()
1266 {
1267 FullyBuild();
1268 }
1269
1270 void FullyBuild()
1271 {
1272 array<string> excludes = OnDebugSpawnBuildExcludes();
1273 array<ConstructionPart> parts = GetConstruction().GetConstructionParts().GetValueArray();
1274
1275 Man p;
1276
1277 #ifdef SERVER
1278 array<Man> players = new array<Man>;
1279 g_Game.GetWorld().GetPlayerList(players);
1280 if (players.Count())
1281 p = players[0];
1282 #else
1283 p = g_Game.GetPlayer();
1284 #endif
1285
1286 foreach (ConstructionPart part : parts)
1287 {
1288 bool excluded = false;
1289 string partName = part.GetPartName();
1290 if (excludes)
1291 {
1292 foreach (string exclude : excludes)
1293 {
1294 if (partName.Contains(exclude))
1295 {
1296 excluded = true;
1297 break;
1298 }
1299 }
1300 }
1301
1302 if (!excluded)
1303 {
1304 OnPartBuiltServer(p, partName, AT_BUILD_PART);
1305 }
1306 }
1307
1308 GetConstruction().UpdateVisuals();
1309 }
1310}
1311
1312void bsbDebugPrint (string s)
1313{
1314#ifdef BSB_DEBUG
1315 PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
1316#else
1317 //Print("" + s); // comment/uncomment to hide/see debug logs
1318#endif
1319}
1320void bsbDebugSpam (string s)
1321{
1322#ifdef BSB_DEBUG_SPAM
1323 PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
1324#else
1325 //Print("" + s); // comment/uncomment to hide/see debug logs
1326#endif
1327}