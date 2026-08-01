// Source: F:/Games/Dayz/scripts/3_game/ppemanager/ppemanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEManagerStatic
{
    static ref PPEManager m_Manager;
    
    static void CreateManagerStatic()
    {
        if (m_Manager)
        {
            Debug.Log(&quot;PPEManagerStatic | CreateManagerStatic - PPEManager already exists&quot;);
            return;
        }
        
        m_Manager = new PPEManager;
