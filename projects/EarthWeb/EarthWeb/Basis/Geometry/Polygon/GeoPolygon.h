/**
	@brief osgEarth::GeoPoint½Ó¿Ú
	@author Hao Luo
	@date 2015/04/13
*/

#ifndef OSGEARTHX_WEB_GEO_POLYGON_H
#define OSGEARTHX_WEB_GEO_POLYGON_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <osgEarthSymbology/Geometry>
#include <EarthWeb/Basis/GeoData/GeoClassType.h>

#define GeoPolygonDispatchImpl EarthObjectDispatchImpl< osgEarth::Symbology::Polygon, GeoPolygon, IEarthObject >

FB_FORWARD_PTR(GeoPolygon)
class GeoPolygon : public GeoPolygonDispatchImpl
{
public:
	GeoPolygon( const std::vector<FB::variant>* pArgs = NULL ) : GeoPolygonDispatchImpl(GEODATA_POLYGON)
	{
		registerMethod("addPoint", make_method(this, &GeoPolygon::addPoint));
		registerMethod("size", make_method(this, &GeoPolygon::size));
		registerMethod("x", make_method(this, &GeoPolygon::x));
		registerMethod("y", make_method(this, &GeoPolygon::y));
		registerMethod("z", make_method(this, &GeoPolygon::z));
	}
	virtual ~GeoPolygon(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	void addPoint(double x, double y, double z);
	double size();
	double x(int index);
	double y(int index);
	double z(int index);
protected:
private:
};

#endif