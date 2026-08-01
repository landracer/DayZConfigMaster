// Source: F:/Games/Dayz/scripts/1_core/proto/envisual.c Source File
// Extracted from DayZ Code Explorer by Zeroy



proto native vobject GetObject(string name);

proto native void ReleaseObject(vobject object, int flag = 0);

proto native int GetNumAnimFrames(vobject anim);

proto string vtoa(vobject vobj);

 
proto int GetObjectMaterials(vobject object, string materials[]);
 
// dynamic model creation (for dynamic aabb triggers)
//proto void CreateModel(IEntity ent, vector mins, vector maxs);
//proto void RemoveModel(IEntity ent);

proto vobject CreateXOB(int nsurfaces, int nverts[], int numindices[], string materials[]);
proto void UpdateVertsEx(notnull IEntity ent, int surf, vector verts[], float uv[]);
proto void UpdateIndices(vobject obj, int surf, int indices[]);
 
proto native void   SetBone(notnull IEntity ent, int bone, vector angles, vector trans, float scale);
proto native bool   SetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native void   SetBoneGlobal(notnull IEntity ent, int bone, vector mat[4]);
proto native bool   GetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native bool   GetBoneLocalMatrix(notnull IEntity ent, int bone, vector mat[4]);
 
proto native void   SetAnimFrame(notnull IEntity ent, int slot, float frame);

class BoneMask
{
    int Mask[8]
