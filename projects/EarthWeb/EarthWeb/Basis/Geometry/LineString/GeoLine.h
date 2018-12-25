/**
	@brief osgEarth::GeoPoint½Ó¿Ú
	@author Hao Luo
	@date 2015/04/13
*/

#ifndef OSGEARTHX_WEB_GEO_LINE_H
#define OSGEARTHX_WEB_GEO_LINE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <osgEarthSymbology/Geometry>
#include <EarthWeb/Basis/GeoData/GeoClassType.h>

#define GeoLineDispatchImpl EarthObjectDispatchImpl< osgEarth::Symbology::LineString, GeoLine, IEarthObject >

FB_FORWARD_PTR(GeoLine)
class GeoLine : public GeoLineDispatchImpl
{
public:
	GeoLine( const std::vector<FB::variant>* pArgs = NULL ) : GeoLineDispatchImpl( GEODATA_LINE )
	{
		registerMethod("addPoint", make_method(this, &GeoLine::addPoint));
		registerMethod("size", make_method(this, &GeoLine::size));
		registerMethod("x", make_method(this, &GeoLine::x));
		registerMethod("y", make_method(this, &GeoLine::y));
		registerMethod("z", make_method(this, &GeoLine::z));
		registerMethod("popPoint", make_method(this, &GeoLine::popPoint));
		registerMethod("setX", make_method(this, &GeoLine::setX));
		registerMethod("setY", make_method(this, &GeoLine::setY));
		registerMethod("setZ", make_method(this, &GeoLine::setZ));
	}
	virtual ~GeoLine(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	void addPoint(double x, double y, double z);
	void popPoint();
	double size();
	double x(int index);
	double y(int index);
	double z(int index);
	void setX(int index,double x);
	void setY(int index,double y);
	void setZ(int index,double z);
protected:
private:
};

#endif