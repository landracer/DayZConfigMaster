// ======================================================================
// File: debugtextworldspace_8c_source.html
// Category: other
// ======================================================================

class DebugTextWorldSpace: DebugText
{
// Instantiate using the Create method
private void DebugTextWorldSpace();
void ~DebugTextWorldSpace();
proto external vector GetPosition();
proto external void SetPosition(float x, float y, float z);
proto external void SetTransform(vector mat[4]);
proto void GetTransform(out vector mat[4]);
static proto DebugTextWorldSpace Create(string text, DebugTextFlags flags, float x, float y, float z, float size = 20.0, int color = 0xFFFFFFFF, int bgColor = 0x00000000, int priority = 1000);
static proto DebugTextWorldSpace CreateInWorld(string text, DebugTextFlags flags, vector transform[4], float size = 20.0, int color = 0xFFFFFFFF, int bgColor = 0x00000000, int priority = 1000);
}