// ======================================================================
// File: jsondatadlcinfo_8c_source.html
// Category: other
// ======================================================================

class JsonDataDLCList : Managed
{
ref array<ref JsonDataDLCInfo> DLCs;
};
class JsonDataDLCInfo : Managed
{
string Name;
string HeaderText;
string DescriptionText;
string VideoFileName;
};