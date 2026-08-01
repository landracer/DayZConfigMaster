// Source: F:/Games/Dayz/scripts/3_game/autotest/autotestfixture.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AutoTestFixture
{
    private static const string REPORT_FILE_NAME_TEMPLATE = &quot;$mission:Autotest_%1.xml&quot;;
    private static const string DATETIME_FORMAT = &quot;%1%2%3T%4%5%6Z&quot;;
 
    private static string m_WorldName;
 
    static bool SaveXMLReport(string data, out string errorMessage)
    {       
        int year, month, day, hour, minute, second;
        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);
        string datetimeUTC  = string.Format(DATETIME_FORMAT, year, month.ToStringLen(2), day.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2));
        string filename     = string.Format(REPORT_FILE_NAME_TEMPLATE, datetimeUTC);
 
        FileHandle handle = OpenFile(filename, FileMode.WRITE);
        if (handle == 0)
        {
            errorMessage = string.Format(&quot;Cannot open file \&quot;%1\&quot; for writing&quot;, filename);
            return false;
        }
 
        FPrint(handle, data);
        CloseFile(handle);
 
        return true;
