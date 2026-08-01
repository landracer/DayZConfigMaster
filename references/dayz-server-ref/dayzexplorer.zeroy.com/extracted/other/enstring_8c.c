// ======================================================================
// File: enstring_8c_source.html
// Category: other
// ======================================================================

class string
{
static const string Empty;
proto native int ToInt();
proto native int HexToInt();
proto native float ToFloat();
proto vector ToVector();
vector BeautifiedToVector()
{
string copy = value;
copy.Replace("<", "");
copy.Replace(">", "");
copy.Replace(",", " ");
return copy.ToVector();
}
proto native int ToAscii();
proto native typename ToType();
static proto string ToString(void var, bool type = false, bool name = false, bool quotes = true);
proto string Substring(int start, int len);
string SubstringInverted( string string_to_split, int position_start, int position_end )
{
string first_half = string_to_split.Substring(0, position_start);
string second_half = string_to_split.Substring( position_end, string_to_split.Length() - position_end );
string result = first_half + second_half;
return result;
}
proto string SubstringUtf8(int startChar, int len);
proto int Replace(string sample, string replace);
proto int ToLower();
proto int ToUpper();
proto native int Length();
proto native int LengthUtf8();
proto native int Hash();
proto native int IndexOf(string sample);
proto native int LastIndexOf(string sample);
proto native int IndexOfFrom(int start, string sample);
bool Contains(string sample)
{
return value.IndexOf(sample) != -1;
}
proto string Trim();
proto int TrimInPlace();
proto int ParseStringEx(out string token);
proto int ParseString(out string tokens[]);
void Split(string sample, out array<string> output)
{
int txt_len = 0;
int sep_pos = -1;
int nxt_sep_pos = 0;
string text = "";
bool line_end = false;
while (line_end == false)
{
sep_pos = sep_pos + txt_len + 1;
nxt_sep_pos = value.IndexOfFrom(sep_pos, sample);
if ( nxt_sep_pos == -1 )
{
nxt_sep_pos = value.Length();
line_end = true;
}
txt_len = nxt_sep_pos - sep_pos;
if ( txt_len > 0 )
{
text = value.Substring(sep_pos, txt_len);
output.Insert(text);
}
}
}
// !Joins array of strings into a single string, separated by &#39;separator&#39;. Inverse of Split
static string Join(string separator, notnull TStringArray tokens)
{
string output;
foreach (int i, string s: tokens)
{
if (i != 0)
output += separator;
output += s;
}
return output;
}
proto string Get(int index);
proto void Set(int index, string input);
#ifdef DIAG_DEVELOPER
void OldSet(int n, string _value)
{
string pre = value.Substring(0, n);
n += 1;
int length = value.Length() - n;
string post = value.Substring(n, length);
value = pre + _value + post;
}
#endif
proto void Insert(int index, string input);
static proto string Format(string fmt, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
};