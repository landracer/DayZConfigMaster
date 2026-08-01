// ======================================================================
// File: param_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
class Param: Managed
{
bool Serialize(Serializer ctx)
{
return false;
}
bool Deserializer(Serializer ctx)
{
return false;
}
};
class Param1<Class T1> extends Param
{
T1 param1;
void Param1(T1 p1)
{
param1 = p1;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1);
}
};
class Param2<Class T1, Class T2> extends Param
{
T1 param1;
T2 param2;
void Param2(T1 p1, T2 p2)
{
param1 = p1;
param2 = p2;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2);
}
};
class Param3<Class T1, Class T2, Class T3> extends Param
{
T1 param1;
T2 param2;
T3 param3;
void Param3(T1 p1, T2 p2, T3 p3)
{
param1 = p1;
param2 = p2;
param3 = p3;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3);
}
};
class Param4<Class T1, Class T2, Class T3, Class T4> extends Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
void Param4(T1 p1, T2 p2, T3 p3, T4 p4)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4);
}
};
class Param5<Class T1, Class T2, Class T3, Class T4, Class T5> extends Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
void Param5(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5);
}
};
class Param6<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6> extends Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
T6 param6;
void Param6(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
param6 = p6;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6);
}
};
class Param7<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7>: Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
T6 param6;
T7 param7;
void Param7(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
param6 = p6;
param7 = p7;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7);
}
};
class Param8<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7, Class T8>: Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
T6 param6;
T7 param7;
T8 param8;
void Param8(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
param6 = p6;
param7 = p7;
param8 = p8;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7) && ctx.Write(param8);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7) && ctx.Read(param8);
}
};
class Param9<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7, Class T8, Class T9>: Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
T6 param6;
T7 param7;
T8 param8;
T9 param9;
void Param9(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
param6 = p6;
param7 = p7;
param8 = p8;
param9 = p9;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7) && ctx.Write(param8) && ctx.Write(param9);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7) && ctx.Read(param8) && ctx.Read(param9);
}
};
class Param10<Class T1, Class T2, Class T3, Class T4, Class T5, Class T6, Class T7, Class T8, Class T9, Class T10>: Param
{
T1 param1;
T2 param2;
T3 param3;
T4 param4;
T5 param5;
T6 param6;
T7 param7;
T8 param8;
T9 param9;
T10 param10;
void Param10(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
{
param1 = p1;
param2 = p2;
param3 = p3;
param4 = p4;
param5 = p5;
param6 = p6;
param7 = p7;
param8 = p8;
param9 = p9;
param10 = p10;
}
override bool Serialize(Serializer ctx)
{
return ctx.Write(param1) && ctx.Write(param2) && ctx.Write(param3) && ctx.Write(param4) && ctx.Write(param5) && ctx.Write(param6) && ctx.Write(param7) && ctx.Write(param8) && ctx.Write(param9) && ctx.Write(param10);
}
override bool Deserializer(Serializer ctx)
{
return ctx.Read(param1) && ctx.Read(param2) && ctx.Read(param3) && ctx.Read(param4) && ctx.Read(param5) && ctx.Read(param6) && ctx.Read(param7) && ctx.Read(param8) && ctx.Read(param9) && ctx.Read(param10);
}
};