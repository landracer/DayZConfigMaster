// Source: F:/Games/Dayz/scripts/2_gamelib/entities/scriptlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE
 
[EditorAttribute(&quot;box&quot;, &quot;GameLib/Scripted&quot;, &quot;Script light&quot;, &quot;-0.25 -0.25 -0.25&quot;, &quot;0.25 0.25 0.25&quot;, &quot;255 0 0 255&quot;)]
class ScriptLightClass
{
 
}
 
ScriptLightClass ScriptLightSource;
 
class ScriptLight: GenericEntity
{
    [Attribute(&quot;1&quot;, &quot;flags&quot;, &quot;Flags&quot;, &quot;&quot;, { ParamEnum(&quot;Point&quot;, &quot;1&quot;), ParamEnum(&quot;Spot&quot;, &quot;2&quot;), ParamEnum(&quot;Directional&quot;, &quot;4&quot;) } )]
    int Flags;
    [Attribute(&quot;1&quot;, &quot;combobox&quot;, &quot;Type&quot;, &quot;&quot;, { ParamEnum(&quot;Point&quot;, &quot;1&quot;), ParamEnum(&quot;Spot&quot;, &quot;2&quot;), ParamEnum(&quot;Directional&quot;, &quot;3&quot;) } )]
    int Type;
    [Attribute(&quot;1&quot;, &quot;editbox&quot;, &quot;Radius&quot;, &quot;&quot;, NULL )]
    float Radius;
    [Attribute(&quot;1 1 1&quot;, &quot;color&quot;, &quot;Color&quot;, &quot;&quot;, NULL )]
    vector Color;
    [Attribute(&quot;1&quot;, &quot;editbox&quot;, &quot;Intensity&quot;, &quot;&quot;, NULL )]
    float Intensity;
    HLIGHT m_light;
    
    void ScriptLight(IEntitySource src, IEntity parent)
    {
        //SetFlags(this, EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE);
        m_light = AddLight(this, LightType.POINT, LightFlags.DYNAMIC|LightFlags.CASTSHADOW, Radius, Color * Intensity);
    }
 
    void ~ScriptLight()
    {
        if(m_light)
        {
            RemoveLight(m_light);
        }
    }
}
 
#endif
