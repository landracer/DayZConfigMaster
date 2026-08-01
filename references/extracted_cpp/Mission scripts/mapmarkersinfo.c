// Source: F:/Games/Dayz/scripts/5_mission/gui/mapmarkersinfo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MapMarkerTypes
{
    protected static ref map<int,string> m_MarkerTypes;
    
    static void Init()
    {
        m_MarkerTypes = new map<int,string>;
        
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_BORDER_CROSS,&quot;\\DZ\\gear\\navigation\\data\\map_border_cross_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_BROADLEAF,&quot;\\DZ\\gear\\navigation\\data\\map_broadleaf_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_CAMP,&quot;\\DZ\\gear\\navigation\\data\\map_camp_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_FACTORY,&quot;\\DZ\\gear\\navigation\\data\\map_factory_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_FIR,&quot;\\DZ\\gear\\navigation\\data\\map_fir_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_FIREDEP,&quot;\\DZ\\gear\\navigation\\data\\map_firedep_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_GOVOFFICE,&quot;\\DZ\\gear\\navigation\\data\\map_govoffice_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_HILL,&quot;\\DZ\\gear\\navigation\\data\\map_hill_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_MONUMENT,&quot;\\DZ\\gear\\navigation\\data\\map_monument_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_PALM,&quot;\\DZ\\gear\\navigation\\data\\map_palm_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_POLICE,&quot;\\DZ\\gear\\navigation\\data\\map_police_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_STATION,&quot;\\DZ\\gear\\navigation\\data\\map_station_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_STORE,&quot;\\DZ\\gear\\navigation\\data\\map_store_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_TOURISM,&quot;\\DZ\\gear\\navigation\\data\\map_tourism_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_TRANSMITTER,&quot;\\DZ\\gear\\navigation\\data\\map_transmitter_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_TSHELTER,&quot;\\DZ\\gear\\navigation\\data\\map_tshelter_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_TSIGN,&quot;\\DZ\\gear\\navigation\\data\\map_tsign_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_VIEWPOINT,&quot;\\DZ\\gear\\navigation\\data\\map_viewpoint_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_VINEYARD,&quot;\\DZ\\gear\\navigation\\data\\map_vineyard_ca.paa&quot;);
        RegisterMarkerType(eMapMarkerTypes.MARKERTYPE_MAP_WATERPUMP,&quot;\\DZ\\gear\\navigation\\data\\map_waterpump_ca.paa&quot;);
