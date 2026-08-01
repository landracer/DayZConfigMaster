// Source: F:/Games/Dayz/scripts/1_core/proto/proto.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
 
/*
Function/method modifiers:
proto   - prototyping of internal function (C++ side)
native - native call convention of internal function (C++ side)
volatile - internal function that may call back to script (hint for
  compiler that context need to be saved on stack)
private - function may not be called from script
event - hint for tools that the function should be exposed as
 Entity script event.
 
Variable modifiers:
owned - modifier for returing internal functions. Tells to script-VM,
that returning variable (string or array) must not be released
out - modifier for function parameters. It tells that variable will
 be changed by function call (used mainly by internal functions)
inout - modifier for function parameters. It tells that variable will
 be used and then changed by function call (used mainly by internal functions)
 
const - constants. May not be modified.
reference - hint for tools (Material editor), that the variable may be used
 as parameter in material
*/
 
 
/*===================================================================*/
/*                          Enforce engine API                       */
/*===================================================================*/
 
//placeholder
class AnimEvent
{
    int type;
    int slot;
