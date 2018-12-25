/**
	@brief osgEarth::GeoPoint½Ó¿Ú
	@author Hao Luo
	@date 2015/04/13
*/

#ifndef OSGEARTHX_WEB_GEO_POINT_H
#define OSGEARTHX_WEB_GEO_POINT_H 1

#include <EarthWeb/Earth/PtrEarthDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <osgEarth/GeoData>
#include <EarthWeb/Basis/GeoData/GeoClassType.h>

#define GeoPointDispatchImpl PtrEarthDispatchImpl< GeoClassType, osgEarth::GeoPoint, GeoPoint, IEarthDispatch >

FB_FORWARD_PTR(GeoPoint)
class GeoPoint : public GeoPointDispatchImpl
{
public:
	GeoPoint( const std::vector<FB::variant>* pArgs = NULL ) : GeoPointDispatchImpl( GEO_POINT )
	{
		registerProperty( "x", make_property( this, & GeoPoint::get_x, & GeoPoint::put_x ) );
		registerProperty( "y", make_property( this, & GeoPoint::get_y, & GeoPoint::put_y ) );
		registerProperty( "z", make_property( this, & GeoPoint::get_z, & GeoPoint::put_z ) );
		registerProperty("SRS", make_property(this, &GeoPoint::get_SRS, &GeoPoint::put_SRS));
		registerProperty("altitudeMode", make_property(this, &GeoPoint::get_altitudeMode, &GeoPoint::put_altitudeMode));
		
	}
	virtual ~GeoPoint(){}

	void put_x( double dX );
	double get_x();
	void put_y( double dY );
	double get_y();
	void put_z( double dZ );
	double get_z();
	FB::JSAPIPtr get_SRS();
	void put_SRS(IEarthObjectPtr ptrObject);
	unsigned int get_altitudeMode();
	void put_altitudeMode(unsigned int md);

protected:
private:
};

#endif