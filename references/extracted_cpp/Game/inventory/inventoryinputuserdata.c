// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/inventoryinputuserdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class InventoryInputUserData
{
    static void SerializeMove(ParamsWriteContext ctx, int type, notnull InventoryLocation src, notnull InventoryLocation dst)
    {
        ctx.Write(INPUT_UDT_INVENTORY);
        ctx.Write(type);
        src.WriteToContext(ctx);
        dst.WriteToContext(ctx);
