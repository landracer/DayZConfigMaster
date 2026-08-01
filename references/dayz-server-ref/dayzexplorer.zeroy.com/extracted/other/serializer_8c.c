// ======================================================================
// File: serializer_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
class Serializer: Managed
{
proto bool Write(void value_out);
proto bool Read(void value_in);
proto native bool CanWrite();
proto native bool CanRead();
protected void Serializer() {}
protected void ~Serializer() {}
};
class FileSerializer: Serializer
{
void FileSerializer() {}
void ~FileSerializer() {}
proto native bool Open(string path, FileMode mode = FileMode.READ);
proto native bool IsOpen();
proto native void Close();
}