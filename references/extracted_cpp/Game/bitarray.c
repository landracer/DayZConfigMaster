// Source: F:/Games/Dayz/scripts/3_game/tools/bitarray.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//this class allows for creation and manipulation of arrays which hold unique IDs, and allow for bitwise-like operation over these IDs
//essentially, this extends single INT to any legth when it comes to holding and manipulating individual bits
 
const int BIT_INT_SIZE = 32;
class BitArray
{   
    static int IDToIndex(int id)
    {
        int m_NumOfItems = BitArray.IDToNumOfItems(id);
        int index   = (id - (m_NumOfItems * BIT_INT_SIZE)) - 1;
        return index;
