/**********************************************************\

  Auto-generated EarthWebAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "BrowserHost.h"
#include "EarthWeb.h"

#ifndef H_EarthWebAPI
#define H_EarthWebAPI

//osgEarthX_Web
#include <EarthWeb/Earth/IEarthDispatch.h>
#include <EarthWeb/EarthMap/EarthMap.h>

#include <EarthWeb/Externals/Graticule/Graticule.h>
#include <EarthWeb/Externals/Sky/Sky.h>
#include <EarthWeb/Externals/Viewpoint/Viewpoint.h>
#include <EarthWeb/Annotation/Place/Place.h>
#include <osgEarthUtil/MeasureTool>
#include <EarthWeb/Basis/Geometry/LineString/GeoLine.h>

class EarthWebAPI;

class MyMeasureToolCallback : public osgEarth::Util::MeasureToolHandler::MeasureToolEventHandler
{
public:
	MyMeasureToolCallback(EarthWebAPI * obj);
	virtual void onDistanceChanged(osgEarth::Util::MeasureToolHandler* sender, double distance);
private:
	EarthWebAPI* _obj;
};

class EarthWebAPI : 
	public IEarthDispatch
{
public:
	void registerEarth()
	{
		registerMethod("run",make_method(this,&EarthWebAPI::run));
		
		registerMethod("getClassFactory",make_method(this,&EarthWebAPI::getClassFactory));
		registerMethod("getEarthRoot",make_method(this,&EarthWebAPI::getEarthRoot));
		registerMethod("getEarthMap",make_method(this,&EarthWebAPI::getEarthMap));
		registerMethod("getViewPoint",make_method(this,&EarthWebAPI::getViewPoint));

		registerMethod( "setActiveSky", make_method( this, &EarthWebAPI::setActiveSky ) );
		registerMethod( "removeActiveSky", make_method( this, &EarthWebAPI::removeActiveSky ) );
		registerMethod( "setHomeViewpoint", make_method( this, &EarthWebAPI::setHomeViewpoint ) );
		registerMethod( "setActiveGraticule", make_method( this, &EarthWebAPI::setActiveGraticule ) );
		registerMethod( "removeActiveGraticule", make_method( this, &EarthWebAPI::removeActiveGraticule ) );
		registerMethod("beginFly", make_method(this, &EarthWebAPI::beginFly));
		registerMethod("stopFly", make_method(this, &EarthWebAPI::stopFly));
		registerMethod("beginRotate", make_method(this, &EarthWebAPI::beginRotate));
		registerMethod("beginFocusRotate", make_method(this, &EarthWebAPI::beginFocusRotate));
		registerMethod("beginPitchRotate", make_method(this, &EarthWebAPI::beginPitchRotate));
		registerMethod("stopRotate", make_method(this, &EarthWebAPI::stopRotate));
		registerMethod("beginFlybyline", make_method(this, &EarthWebAPI::beginFlybyline));
		registerMethod("stopFlybyline", make_method(this, &EarthWebAPI::stopFlybyline));
		registerMethod("setFocus", make_method(this, &EarthWebAPI::setFocus));
		registerMethod("print", make_method(this, &EarthWebAPI::print));
		registerMethod("getLocationZ", make_method(this, &EarthWebAPI::getLocationZ));
		registerMethod("heatMap", make_method(this, &EarthWebAPI::heatMap));
		registerMethod("startViewshed", make_method(this, &EarthWebAPI::startViewshed));
		registerMethod("stopViewshed", make_method(this, &EarthWebAPI::stopViewshed));
		registerMethod("getScreenXY", make_method(this, &EarthWebAPI::getScreenXY));
		registerMethod("setFog", make_method(this, &EarthWebAPI::setFog));
		registerMethod("setRain", make_method(this, &EarthWebAPI::setRain));
		registerMethod("setSnow", make_method(this, &EarthWebAPI::setSnow));
		registerMethod("setTerrainEffect", make_method(this, &EarthWebAPI::setTerrainEffect));
		registerMethod("LineOfSight", make_method(this, &EarthWebAPI::LineOfSight));
		registerMethod("RadialLineOfSight", make_method(this, &EarthWebAPI::RadialLineOfSight));
		registerMethod("TerrainProfile", make_method(this, &EarthWebAPI::TerrainProfile));
		registerMethod("ClearCalResult", make_method(this, &EarthWebAPI::ClearCalResult));
		registerMethod("clearScene", make_method(this, &EarthWebAPI::clearScene));
		registerMethod("addMask", make_method(this, &EarthWebAPI::addMask));
		registerMethod("removeMask", make_method(this, &EarthWebAPI::removeMask));
		registerMethod("fullScreen", make_method(this, &EarthWebAPI::fullScreen));
		registerMethod("showTip", make_method(this, &EarthWebAPI::showTip));
		registerMethod("clearTip", make_method(this, &EarthWebAPI::clearTip));
		registerMethod("setSelectColor", make_method(this, &EarthWebAPI::setSelectColor));
	}

public:
	//IEarthJSAPI
	virtual bool equal( IEarthDispatchPtr ptr );

	//osgEarthX
	void run();

	//osgEarthX_Web objects
public:
	//获取类创建对象
	FB::JSAPIPtr getClassFactory();
	FB::JSAPIPtr getEarthRoot();
	FB::JSAPIPtr getEarthMap();
	FB::JSAPIPtr getViewPoint();
	void setActiveSky( SkyPtr ptrSky );
	void removeActiveSky();
	void setHomeViewpoint( ViewpointPtr ptrViewpoint );
	void setActiveGraticule( GraticulePtr ptrGraticule );
	void removeActiveGraticule();
	void beginFly();
	void stopFly();
	void beginRotate(double dlon,double dlat,double dele,double drange);
	void beginFocusRotate(double dlon, double dlat, double dele, double dmaxRange,double dminRange);
	void beginPitchRotate(double dlon, double dlat, double dele, double dRange,double dmaxPitch, double dminPitch);

	void stopRotate();
	void beginFlybyline(GeoLinePtr lin, double dEvl, double dHitch, double dSpeed);
	void stopFlybyline();
	void print(std::wstring filePath);
	void clearScene();
	void addMask(std::vector<double> &cords,double h,bool bOpatity);
	void removeMask();
	void fullScreen(bool bfull);
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn EarthWebAPI::EarthWebAPI(const EarthWebPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    EarthWebAPI(const EarthWebPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &EarthWebAPI::echo));
        registerMethod("setOperation", make_method(this, &EarthWebAPI::setOperation));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &EarthWebAPI::get_testString,
                                       &EarthWebAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &EarthWebAPI::get_version));

		registerEarth();
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn EarthWebAPI::~EarthWebAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EarthWebAPI() {};

    EarthWebPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(measure, 2, (const double,const int));
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));
	FB_JSAPI_EVENT(featureClicked, 1, (IEarthObjectPtr));
	FB_JSAPI_EVENT(hoverEnter, 1, (PlacePtr));
	FB_JSAPI_EVENT(hoverLeave, 1, (PlacePtr));
	FB_JSAPI_EVENT(mouseDown, 3, (const double, const double, const double));
	FB_JSAPI_EVENT(mouseUp, 4, (const double, const double, const double, const std::wstring));
	FB_JSAPI_EVENT(mouseMove, 3, (const double, const double, const double));
	FB_JSAPI_EVENT(mouseDoubleClick, 3, (const double, const double, const double));
	FB_JSAPI_EVENT(tipClicked, 3, (PlacePtr,const std::wstring,const int));

    // Method test-event
    void setOperation(const int oper);
	void setFocus();

	double getLocationZ(double dlon, double dlat);
	std::vector<double> heatMap(std::string pstrFileName, int opacity, std::vector<double> &LonLat);
		//void getLocationZ(const double dlon, const double dlat);
	void startViewshed(double dlon, double dlat, double dAlt, double dHead, double dPitch, double dRange);
	void stopViewshed();
	std::vector<double> getScreenXY(double x,double y,double z);
	void setFog(bool bFog);
	void setRain(bool bRain);
	void setSnow(bool bSnow);
	void setTerrainEffect(bool bEffect);
	void LineOfSight(double dlon, double dlat, double dAlt, double dlonEnd, double dlatEnd, double dAltEnd);
	void RadialLineOfSight(double dlon,double dlat,double dAlt, double dr);
	void TerrainProfile(double dlon, double dlat, double dlonEnd, double dlatEnd, bool bShow);
	void ClearCalResult();
	void showTip(double lon, double lat, double alt, const std::wstring &strContent);
	void clearTip();
	void setSelectColor(const std::wstring& col,bool bCol);

private:
    EarthWebWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
	osg::ref_ptr<MyMeasureToolCallback> m_measureCallback;
};

#endif // H_EarthWebAPI

