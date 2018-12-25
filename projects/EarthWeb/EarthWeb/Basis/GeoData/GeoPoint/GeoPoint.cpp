#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>
#include <EarthWeb/Utils/StaticUtils/EarthObjectStaticUtils.h>
#include <EarthWeb\Basis\GeoData\SpatialReference\SpatialReference.h>

void GeoPoint::put_x( double dX )
{
	getOrCreateInnerObject()->x() = dX;
}

double GeoPoint::get_x()
{
	return getOrCreateInnerObject()->x();
}

void GeoPoint::put_y( double dY )
{
	getOrCreateInnerObject()->y() = dY;
}

double GeoPoint::get_y()
{
	return getOrCreateInnerObject()->y();
}

void GeoPoint::put_z( double dZ )
{
	getOrCreateInnerObject()->z() = dZ;
}

double GeoPoint::get_z()
{
	return getOrCreateInnerObject()->z();
}

FB::JSAPIPtr GeoPoint::get_SRS()
{
	osgEarth::SpatialReference * pObject = const_cast<osgEarth::SpatialReference *>(getOrCreateInnerObject()->getSRS());
	return EarthObjectStaticUtils::createInstance(SPATIAL_REFERENCE, pObject);
}

void GeoPoint::put_SRS(IEarthObjectPtr ptrObject)
{
	if (!ptrObject)
	{
		return;
	}

	SpatialReference* pSpatialRef = dynamic_cast< SpatialReference* >(ptrObject.get());
	osgEarth::SpatialReference * pObject = pSpatialRef->getOrCreateInnerObject();
	
	getOrCreateInnerObject()->setSRS(pObject);

}

unsigned int GeoPoint::get_altitudeMode()
{
	return getOrCreateInnerObject()->altitudeMode();
}

void GeoPoint::put_altitudeMode(unsigned int md)
{
	getOrCreateInnerObject()->altitudeMode() = (osgEarth::AltitudeMode)md;
}
