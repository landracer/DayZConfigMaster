// Source: F:/Games/Dayz/scripts/1_core/proto/serializer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------------------------------------------------------
class Serializer: Managed
{
    proto bool Write(void value_out);
    proto bool Read(void value_in);
 
    proto native bool CanWrite();
    proto native bool CanRead();
    
    protected void Serializer() {}
    protected void ~Serializer() {}
