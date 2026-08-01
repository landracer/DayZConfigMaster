// ======================================================================
// File: core_2inherited_2plant_8c_source.html
// Category: other
// ======================================================================

/*enum PlantType
{
TREE_HARD = 0,
TREE_SOFT = 1,
BUSH_HARD = 2,
BUSH_SOFT = 3,
}*/
//-----------------------------------------------------------------------------
class TreeHard : PlantSuper
{
override void OnTreeCutDown(EntityAI cutting_entity)
{
g_Game.RPCSingleParam(cutting_entity, PlantType.TREE_HARD, null, true);
}
override bool IsTree()
{
return true;
}
override bool CanBeAutoDeleted()
{
return false;
}
override bool HasPlayerCollisionSound()
{
return false;
}
};
//-----------------------------------------------------------------------------
class TreeSoft : PlantSuper
{
override void OnTreeCutDown(EntityAI cutting_entity)
{
g_Game.RPCSingleParam(cutting_entity, PlantType.TREE_SOFT, null, true);
}
override bool IsTree()
{
return true;
}
override bool CanBeAutoDeleted()
{
return false;
}
override bool HasPlayerCollisionSound()
{
return false;
}
};
//-----------------------------------------------------------------------------
class BushHard : PlantSuper
{
override void OnTreeCutDown(EntityAI cutting_entity)
{
g_Game.RPCSingleParam(cutting_entity, PlantType.BUSH_HARD, null, true);
}
override bool IsBush()
{
return true;
}
override bool CanBeAutoDeleted()
{
return false;
}
};
//-----------------------------------------------------------------------------
class BushSoft : PlantSuper
{
override void OnTreeCutDown(EntityAI cutting_entity)
{
g_Game.RPCSingleParam(cutting_entity, PlantType.BUSH_SOFT, null, true);
}
override bool IsBush()
{
return true;
}
override bool CanBeAutoDeleted()
{
return false;
}
};