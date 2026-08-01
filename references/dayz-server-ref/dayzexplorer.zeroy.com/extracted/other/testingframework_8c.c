// ======================================================================
// File: testingframework_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
class Test
{
string Suite;
int TimeoutS;
int TimeoutMs;
int SortOrder;
void Test(string suite, int timeoutS = 0, int timeoutMs = 0, int sortOrder = 0)
{
Suite = suite;
TimeoutS = timeoutS;
TimeoutMs = timeoutMs;
SortOrder = sortOrder;
}
}
//-----------------------------------------------------------------------------
enum EStage
{
Setup,
Main,
TearDown
}
//-----------------------------------------------------------------------------
class Step
{
EStage Stage;
void Step(EStage stage = EStage.Main)
{
Stage = stage;
}
}
//-----------------------------------------------------------------------------
class TestHarness : Managed
{
proto native static bool Run();
proto static string Report();
proto native static int GetNSuites();
proto native static TestSuite GetSuite(int handle);
proto native static TestSuite ActiveSuite();
proto native static bool Finished();
proto native static void Begin();
proto native static void End();
}
//-----------------------------------------------------------------------------
class TestSuite : Managed
{
proto native void SetResult(TestResultBase res);
proto native int GetNTests();
proto native TestBase GetTest(int handle);
proto native void SetEnabled(bool val);
proto native bool IsEnabled();
proto string GetName();
protected void OnInit();
}
//-----------------------------------------------------------------------------
class TestBase : Managed
{
proto native void SetResult(TestResultBase res);
proto native TestResultBase GetResult();
proto native void SetEnabled(bool val);
proto native bool IsEnabled();
proto string GetName();
}
//-----------------------------------------------------------------------------
class TestResultBase : Managed
{
bool Failure() { return NativeFailure(); }
string FailureText() { return NativeFailureText(); }
// Script forwarding to cpp. Otherwise the script overloading wouldn&#39;t be able
// to call the native base implementation.
// ----------------- vvv -----------------
proto native bool NativeFailure();
proto native string NativeFailureText();
// ----------------- ^^^ -----------------
}
//-----------------------------------------------------------------------------
// EXAMPLES
//-----------------------------------------------------------------------------
/*
//-----------------------------------------------------------------------------
class TestBoolResult : TestResultBase
{
bool Value;
void TestBoolResult(bool val) { Value = val; }
override bool Failure() { return !Value; }
override string FailureText()
{
// junit kind of error report. (simple)
return "<failure type=\"BoolResult\">Failed</failure>";
}
}
//-----------------------------------------------------------------------------
class MyHarness : TestHarness
{
}
//-----------------------------------------------------------------------------
class MyTestSuite : TestSuite
{
int cnt;
[Step(EStage.Setup)]
void Prep()
{
Print("MyTestSuite::Prep");
cnt = 3;
}
[Step(EStage.Setup)]
bool Count()
{
--cnt;
Print("MyTestSuite::Count: cnt=" + cnt);
return cnt == 0;
}
[Step(EStage.TearDown)]
bool CountUp()
{
++cnt;
Print("MyTestSuite::CountUp: cnt=" + cnt);
return cnt == 10;
}
}
//-----------------------------------------------------------------------------
[Test("MyTestSuite")]
TestResultBase MyTest()
{
Print("MyFuncTest");
return new TestBoolResult(true);
}
//-----------------------------------------------------------------------------
[Test("MyTestSuite")]
class MyAsyncTest : TestBase
{
int counter;
[Step(EStage.Main)]
void Set()
{
counter = 10;
}
[Step(EStage.Main)]
bool Pool()
{
Print("AsyncTest::Pool::counter=" + counter);
if(counter == 0)
{
Print("AsyncTest::Pool::Result");
SetResult(new TestBoolResult(false));
return true;
}
Print("AsyncTest::Pool::Progress");
counter--;
return false;
}
}
*/