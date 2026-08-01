// Source: F:/Games/Dayz/scripts/3_game/gui/credits/creditsloader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CreditsLoader
{
    protected static const string JSON_FILE_PATH = &quot;scripts/data/credits.json&quot;;
    
    static void CreateTestJson()
    {
        string nameDepartment   = &quot;Department&quot;;
        string nameSection      = &quot;Section&quot;;
        string nameLine         = &quot;Line&quot;;
        
        JsonDataCredits data = new JsonDataCredits();
        data.Departments = new array<ref JsonDataCreditsDepartment>;
        
        for (int index_dep = 0; index_dep < 3; ++index_dep)
        {
            JsonDataCreditsDepartment department = new JsonDataCreditsDepartment();
            department.Sections = new array<ref JsonDataCreditsSection>();
            department.DepartmentName = (nameDepartment +&quot; &quot;+ index_dep);
            
            for (int index_sec = 0; index_sec < 4; ++index_sec)
            {
                JsonDataCreditsSection section = new JsonDataCreditsSection();
                section.SectionLines = new array<string>;
                section.SectionName = (nameSection +&quot; &quot;+ index_sec);
                
                int linesCount = Math.RandomInt(3, 10);
                for (int i = 0; i < linesCount; ++i)
                {
                    section.SectionLines.Insert(nameLine +&quot; &quot;+ i);
                }   
                
                department.Sections.Insert(section);
            }
            
            data.Departments.Insert(department);
        }
 
        string errorMessage;
        if (!JsonFileLoader<ref JsonDataCredits>.SaveFile(JSON_FILE_PATH, data, errorMessage))
            ErrorEx(errorMessage);
