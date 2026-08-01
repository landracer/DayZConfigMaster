// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/optics.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//class Crossbow_RedpointOptic : ItemOptics {};     //! Unused?!
class LongrangeOptic : ItemOptics {};               
class SportingOptic : ItemOptics {};
class PistolOptic : ItemOptics {};
class GrozaOptic : ItemOptics {};
class ACOGOptic : ItemOptics {};
class ACOGOptic_6x : ItemOptics {};
class KashtanOptic : ItemOptics {};
 
class M68Optic : ItemOptics
{
    override bool IsUsableWithNV()
    {
        return true;
