/**
	@brief osgEarth::SpatialReference½Ó¿Ú
	@author Hao Luo
	@date 2015/04/13
*/

#ifndef OSGEARTHX_WEB_SPATIALREFENCE_H
#define OSGEARTHX_WEB_SPATIALREFENCE 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>
#include <osgEarth/GeoData>
#include <EarthWeb/Basis/GeoData/GeoClassType.h>

#define SpatialReferenceDispatchImpl EarthObjectDispatchImpl< osgEarth::SpatialReference, SpatialReference, IEarthObject >

FB_FORWARD_PTR(SpatialReference)
class SpatialReference : public SpatialReferenceDispatchImpl
{
public:
	SpatialReference( const std::vector<FB::variant>* pArgs = NULL ) : SpatialReferenceDispatchImpl(SPATIAL_REFERENCE)
	{

	}
	virtual ~SpatialReference(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

protected:
private:
};

#endif