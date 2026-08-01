// ======================================================================
// File: envisual_8c_source.html
// Category: other
// ======================================================================

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
proto native void SetBone(notnull IEntity ent, int bone, vector angles, vector trans, float scale);
proto native bool SetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native void SetBoneGlobal(notnull IEntity ent, int bone, vector mat[4]);
proto native bool GetBoneMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native bool GetBoneLocalMatrix(notnull IEntity ent, int bone, vector mat[4]);
proto native void SetAnimFrame(notnull IEntity ent, int slot, float frame);
class BoneMask
{
int Mask[8]
}
enum AnimFlags
{
ONCE,
BLENDOUT,
USER,
RESET,
FORCEFPS,
NOANIMEND,
NOANIMHOOKS
};
proto native void SetAnimSlot(notnull IEntity ent, int slot, vobject anim, float blendin, float blendout, BoneMask mask, int fps, AnimFlags flags);
// changes a mask and does the blending if the blendin is set
proto native void ChangeAnimSlotMask(notnull IEntity ent, int slot, float blendin, BoneMask mask);
// changes animation framerate to fps param
proto native void ChangeAnimSlotFPS(notnull IEntity ent, int slot, int fps);
// set mask for a channel. There are 12 chanels and mask is using first for bits 0..3
proto native void SetAnimMask(notnull IEntity ent, int mask);
// clears chanal mask, return bits which were cleared
proto native int ClearAnimMask(notnull IEntity ent, int mask);
//vrati nastavene bity tech kanalu, ktere maji nastavenou animaci, nejsou na konci
// a pro ktere byla nastavena vstupni maska.
//Je tim mozno se dotazat na stav vice slotu najednou
proto native int IsAnimSlotPlaying(notnull IEntity ent, int mask);
//mask - 16bitu, pro 16 anim slotu. Maximalni hodnota je tedy 0xffff!
//sets how much this morph affect object
proto native bool SetMorphState(notnull IEntity ent, string morph, float value);
//morph name
//value 0...1
enum EmitorParam
{
CONEANGLE,
EMITOFFSET,
VELOCITY,
VELOCITY_RND,
AVELOCITY,
SIZE,
STRETCH,
RANDOM_ANGLE,
RANDOM_ROT,
AIR_RESISTANCE,
AIR_RESISTANCE_RND,
GRAVITY_SCALE,
GRAVITY_SCALE_RND,
BIRTH_RATE,
BIRTH_RATE_RND,
LIFETIME,
LIFETIME_RND,
LIFETIME_BY_ANIM,
ANIM_ONCE,
RAND_FRAME,
EFFECT_TIME,
REPEAT,
CURRENT_TIME,
ACTIVE_PARTICLES,
SORT,
WIND,
SPRING
};
// return total count of active particles in all emitors
// internally does a sum
// HasActiveParticles is better when just needing to check if there are any active
proto native int GetParticleCount(notnull IEntity ent);
// return if there are any active particles
proto bool HasActiveParticle(notnull IEntity ent);
int ParticleGetCount(IEntity ent)
{
return GetParticleCount(ent);
}
bool ParticleHasActive(IEntity ent)
{
return HasActiveParticle(ent);
}
// gets name of defined emitors in a particle effect
// return number of emitors and their names in an array (max defines max. emitors to return)
proto int GetParticleEmitors(notnull IEntity ent, out string emitors[], int max);
// return number of emitors
proto int GetParticleEmitorCount(notnull IEntity ent);
// sets a parametr of the particle emitor
// if the emitor == - 1, it sets the parameter for all emitors
proto void SetParticleParm(notnull IEntity ent, int emitor, EmitorParam parameter, void value);
//gets parameter of particle emitor
proto void GetParticleParm(notnull IEntity ent, int emitor, EmitorParam parameter, out void value);
//gets original parameter of particle emitor
proto void GetParticleParmOriginal(notnull IEntity ent, int emitor, EmitorParam parameter, out void value);
//Force-changes particle emitor position to the current one.
//Used for sudden changes of particle position to avoid spreading emitted
//particles between previous and the new one position
proto native void ResetParticlePosition(notnull IEntity ent);
//Restart particle effect to its default state. This means no
//particles, timer reset and so on. Usefull for implementing
//particle cache.
proto native void RestartParticle(notnull IEntity ent);