// Source: F:/Games/Dayz/scripts/3_game/tools/jsonfileloader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class JsonFileLoader<Class T>
{
    protected static const int READ_FILE_LENGTH = 100000000;
    
    protected static ref JsonSerializer m_Serializer = new JsonSerializer();
    
    static bool LoadFile(string filename, out T data, out string errorMessage)
    {
        if (FileExist(filename))
        {
            FileHandle handle = OpenFile(filename, FileMode.READ);
            if (handle == 0)
            {
                errorMessage = string.Format(&quot;Cannot open file \&quot;%1\&quot; for reading&quot;, filename);
                return false;
            }
            
            string fileContent;
            ReadFile(handle, fileContent, READ_FILE_LENGTH);
 
            CloseFile(handle);
            
            if (!m_Serializer)
                m_Serializer = new JsonSerializer();
            
            string error;
            if (!m_Serializer.ReadFromString(data, fileContent, error))
            {
                errorMessage = string.Format(&quot;Cannot load data from \&quot;%1\&quot;:\n%2&quot;, filename, error);
                return false;
            }
            
            return true;
        }
        else
        {
            errorMessage = string.Format(&quot;File \&quot;%1\&quot; does not exist, check the provided path&quot;, filename, error);
            return false;
        }
